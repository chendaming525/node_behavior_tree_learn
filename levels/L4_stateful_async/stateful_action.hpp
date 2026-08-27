#pragma once
#include "mini_bt/three_node.hpp"

class StatefulAction : public ThreeNode
{
public:
    explicit StatefulAction(const std::string &name) : ThreeNode(name) {}
    virtual mini_bt::NodeStatus onStart() = 0;
    virtual mini_bt::NodeStatus onRunning() = 0;
    virtual void onHalted() {}
    mini_bt::NodeStatus tick() override
    {
        if (curStatus() == mini_bt::NodeStatus::IDLE)
        {
            /* code */
            return onStart();
        }
        else if (curStatus() == mini_bt::NodeStatus::RUNNING)
        {
            /* code */
            return onRunning();
        }
        return curStatus();
    }
    void halt() override
    {
        if (curStatus() == mini_bt::NodeStatus::RUNNING)
        {
            onHalted();
        }
        ThreeNode::halt(); // 置 IDLE
    }
};