#pragma once

#include "mini_bt/three_node.hpp"

#include <memory>
#include <utility>

class DecoratorBase : public ThreeNode
{
public:
  DecoratorBase(const std::string& name, NodeConfig config, std::shared_ptr<ThreeNode> child)
    : ThreeNode(name, std::move(config)), m_child(std::move(child))
  {}

  void setBlackboard(std::shared_ptr<Blackboard> bb) override
  {
    ThreeNode::setBlackboard(bb);
    if(m_child)
    {
      m_child->setBlackboard(bb);
    }
  }

  void resetChild()
  {
    if(!m_child)
    {
      return;
    }
    if(m_child->curStatus() == mini_bt::NodeStatus::RUNNING)
    {
      m_child->halt();
    }
    else
    {
      m_child->setStatus(mini_bt::NodeStatus::IDLE);
    }
  }

  void halt() override
  {
    resetChild();
    ThreeNode::halt();
  }

protected:
  std::shared_ptr<ThreeNode> m_child;
};
