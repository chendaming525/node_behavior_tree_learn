#pragma once

#include "three_node.hpp"

#include <iostream>
#include <string>
#include <utility>

/** Sync leaves for the "enter room" graduation scenario. */

class BatteryOK : public ThreeNode
{
public:
    explicit BatteryOK(std::string name, NodeConfig config = {})
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {};
    }

    mini_bt::NodeStatus tick() override
    {
        std::cout << "[" << getName() << "] battery OK\n";
        return mini_bt::NodeStatus::SUCCESS;
    }
};

/** Door is closed in this demo → always FAILURE (forces Fallback to plan B). */
class DoorOpen : public ThreeNode
{
public:
    explicit DoorOpen(std::string name, NodeConfig config = {})
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {};
    }

    mini_bt::NodeStatus tick() override
    {
        std::cout << "[" << getName() << "] door is CLOSED\n";
        return mini_bt::NodeStatus::FAILURE;
    }
};

class FindKey : public ThreeNode
{
public:
    explicit FindKey(std::string name, NodeConfig config = {})
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {};
    }

    mini_bt::NodeStatus tick() override
    {
        std::cout << "[" << getName() << "] found key\n";
        return mini_bt::NodeStatus::SUCCESS;
    }
};

/**
 * Fails the first (failures_before_success) times, then SUCCESS.
 * Used under Retry to demonstrate decorator + multi-tick recovery.
 */
class FlakyOpenDoor : public ThreeNode
{
public:
    FlakyOpenDoor(std::string name, NodeConfig config = {}, int failures_before_success = 2)
        : ThreeNode(std::move(name), std::move(config)),
          failures_before_success_(failures_before_success)
    {
    }

    static PortsList providedPorts()
    {
        return {};
    }

    mini_bt::NodeStatus tick() override
    {
        ++tries_;
        if (tries_ <= failures_before_success_)
        {
            std::cout << "[" << getName() << "] open failed (try " << tries_ << ")\n";
            return mini_bt::NodeStatus::FAILURE;
        }
        std::cout << "[" << getName() << "] open SUCCESS (try " << tries_ << ")\n";
        return mini_bt::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        // Keep tries_ across Retry attempts so flaky progress is remembered.
        ThreeNode::halt();
    }

private:
    int failures_before_success_ = 2;
    int tries_ = 0;
};

class EnterRoom : public ThreeNode
{
public:
    explicit EnterRoom(std::string name, NodeConfig config = {})
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {};
    }

    mini_bt::NodeStatus tick() override
    {
        std::cout << "[" << getName() << "] entered room\n";
        return mini_bt::NodeStatus::SUCCESS;
    }
};

class SetBlackboardNode : public ThreeNode
{
public:
    SetBlackboardNode(std::string name, NodeConfig config)
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {InputPort<std::string>("value", "value to write"),
                OutputPort<std::string>("text", "blackboard destination")};
    }

    PortsList ports() const override
    {
        return providedPorts();
    }

    mini_bt::NodeStatus tick() override
    {
        std::string value;
        if (!getInput("value", value))
        {
            std::cout << "[" << getName() << "] missing input port: value\n";
            return mini_bt::NodeStatus::FAILURE;
        }

        setOutput("text", value);
        const auto mapped = config().output_ports.find("text");
        std::cout << "[" << getName() << "] output text -> " << mapped->second
                  << " = \"" << value << "\"\n";
        return mini_bt::NodeStatus::SUCCESS;
    }
};

class SaySomethingNode : public ThreeNode
{
public:
    SaySomethingNode(std::string name, NodeConfig config)
        : ThreeNode(std::move(name), std::move(config))
    {
    }

    static PortsList providedPorts()
    {
        return {InputPort<std::string>("message", "message to print")};
    }

    PortsList ports() const override
    {
        return providedPorts();
    }

    mini_bt::NodeStatus tick() override
    {
        std::string msg;
        if (!getInput("message", msg))
        {
            std::cout << "[" << getName() << "] missing input port: message\n";
            return mini_bt::NodeStatus::FAILURE;
        }
        std::cout << "[" << getName() << "] Robot says: " << msg << "\n";
        return mini_bt::NodeStatus::SUCCESS;
    }
};
