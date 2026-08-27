#pragma once

// =============================================================================
// ports.hpp — Port 说明书 + remapping 字符串小工具
//
// 学习顺序（不要一次从上啃到下）：
//   1) PortDirection / PortInfo / PortsList     ← 先只看这三样
//   2) InputPort / OutputPort / portTypeName   ← 如何写出 providedPorts()
//   3) isBlackboardPointer / strip...          ← "{key}" 怎么识别
//   4) convertFromString                       ← 常量 "42" 怎么变成 int
//
// 本文件不创建节点，也不直接读写 Blackboard。
// 真正取值在 three_node.hpp 的 getInput / setOutput。
// =============================================================================

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

// -----------------------------------------------------------------------------
// 1) 数据模型：节点类型对外公开的“口”
// -----------------------------------------------------------------------------

/** 接口的方向：输入：函数参数，输出：返回值。 */
enum class PortDirection
{
    INPUT,
    OUTPUT
};

/**
 * 单个口的静态描述（说明书上的一行）。
 * 运行时节点里通常没有独立的 Port 对象，只有这份描述 + NodeConfig 里的字符串。
 */
struct PortInfo
{
    PortDirection direction;
    std::string type_name;   // 可读类型名，如 "string" / "int"（便于打印/校验）
    std::string description; // 给人看的说明，可空
};

/**
 * 节点类型的完整接口说明书：
 *   口名 -> PortInfo
 *
 * 例：
 *   "message" -> { INPUT,  "string", "..." }
 *   "text"    -> { OUTPUT, "string", "..." }
 *
 * 业务节点通过 static providedPorts() 返回它。
 */
using PortsList = std::map<std::string, PortInfo>;

// -----------------------------------------------------------------------------
// 2) 造说明书条目：InputPort / OutputPort（不是基类，是工厂函数）
// -----------------------------------------------------------------------------

/** 把模板类型 T 转成简短可读名，填进 PortInfo::type_name。 */
template <typename T>
std::string portTypeName()
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return "string";
    }
    else if constexpr (std::is_same_v<T, int>)
    {
        return "int";
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        return "bool";
    }
    else
    {
        return "unknown";
    }
}

/**
 * 声明一个输入口。返回 pair，可直接塞进 PortsList 初始化列表。
 *
 * 用法：
 *   return { InputPort<std::string>("message") };
 *
 * 这里的 "message" 是口名（节点稳定接口），不是 Blackboard key。
 */
template <typename T>
std::pair<std::string, PortInfo> InputPort(
    std::string name, std::string description = "")
{
    const std::string key = std::move(name);
    return {key,
            PortInfo{PortDirection::INPUT, portTypeName<T>(), std::move(description)}};
}

/**
 * 声明一个输出口。语义同 InputPort，只是方向为 OUTPUT。
 *
 * 用法：
 *   return { OutputPort<std::string>("text") };
 *
 * 输出一般必须 remap 到 "{blackboard_key}"（见 three_node::setOutput）。
 */
template <typename T>
std::pair<std::string, PortInfo> OutputPort(
    std::string name, std::string description = "")
{
    const std::string key = std::move(name);
    return {key,
            PortInfo{PortDirection::OUTPUT, portTypeName<T>(), std::move(description)}};
}

// -----------------------------------------------------------------------------
// 3) Remapping 字符串：常量 vs 黑板引用
// -----------------------------------------------------------------------------
//
// NodeConfig 里存的是原始字符串，例如：
//   input_ports["message"]  = "hello"           // 常量
//   input_ports["message"]  = "{door_message}"  // 黑板引用
//   output_ports["text"]    = "{door_message}"  // 输出必须指向黑板
//

/** 是否为黑板指针语法：以 '{' 开头、'}' 结尾，且至少有一个字符在中间。 */
inline bool isBlackboardPointer(const std::string &value)
{
    return value.size() >= 3 && value.front() == '{' && value.back() == '}';
}

/**
 * "{door_message}" -> "door_message"
 * 若不是黑板引用则抛错（输出口配置错时常走这里）。
 */
inline std::string stripBlackboardPointer(const std::string &value)
{
    if (!isBlackboardPointer(value))
    {
        throw std::runtime_error("Port remapping is not a blackboard reference: " + value);
    }
    return value.substr(1, value.size() - 2);
}

// -----------------------------------------------------------------------------
// 4) 常量字符串 -> 目标类型（仅常量输入路径使用）
// -----------------------------------------------------------------------------
//
// getInput 时：
//   - remapping 是 "{key}"  -> blackboard.get<T>(key)   （不走本函数）
//   - remapping 是 "hello"  -> convertFromString<T>(...) （走本函数）
//

template <typename T>
T convertFromString(const std::string &value)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return value;
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        if (value == "true" || value == "1")
        {
            return true;
        }
        if (value == "false" || value == "0")
        {
            return false;
        }
        throw std::runtime_error("Cannot convert to bool: " + value);
    }
    else
    {
        // int / double 等：用流解析；要求整段字符串都能消费完
        std::istringstream input(value);
        T out{};
        input >> out;
        if (input.fail() || !input.eof())
        {
            throw std::runtime_error("Cannot convert port value: " + value);
        }
        return out;
    }
}
