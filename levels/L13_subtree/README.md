# L13 - SubTree 与边界 remapping

## 本关目标

支持 `<SubTree ID="...">`，让一棵 `BehaviorTree` 定义可以被另一棵树引用并重新实例化。

## 核心理解

`SubTree` 不是普通业务节点。它是 XML 构建器的特殊语法：

```text
<SubTree ID="EnterRoomPlan" result="{final_result}"/>
  -> 找到 <BehaviorTree ID="EnterRoomPlan">
  -> 新建一份子树节点对象
  -> 子树内部 {result} 映射到父树 {final_result}
```

## 边界

本关支持：

- 多个 `BehaviorTree` 定义
- `SubTree ID` 查找目标树
- 子 Blackboard 指向父 Blackboard
- 显式 remapping，例如 `result="{final_result}"`
- 递归 SubTree 引用检测

暂不支持：

- `_autoremap`
- 插件和动态库
- 私有 key 规则
- 脚本语言

## 文件

| 文件 | 职责 |
| --- | --- |
| `xml_instantiation.hpp` | Factory 构建器 + SubTree 展开 |
| `blackboard.hpp` | parent/remapping 支持 |
| `l13_subtree.xml` | 主树调用子树 |
| `l13_subtree.cpp` | 运行 SubTree demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l13_subtree
```

## 验收

- 子树内部只认识 `{result}`。
- 父树通过 `{final_result}` 读取子树输出。
- `SubTree` 每次调用都会创建新的节点对象。
- 整棵树最终返回 `SUCCESS`。
