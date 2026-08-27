#pragma once

// =============================================================================
// tree.hpp — L10 SKELETON (fill the TODOs)
//
// Role: package root + blackboard + tick loop as one runnable object.
// L12 XML instantiation should also return Tree.
// =============================================================================

#include "blackboard.hpp"
#include "three_node.hpp"
#include "types.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

class Tree
{
public:
  Tree() = default;

  // TODO:
  // - store root_ / blackboard_
  // - if bb != nullptr, root->setBlackboard(bb)
  Tree(std::shared_ptr<ThreeNode> root, std::shared_ptr<Blackboard> bb)
  {
    (void)root;
    (void)bb;
    throw std::runtime_error("TODO: Tree::Tree(root, bb)");
  }

  [[nodiscard]] std::shared_ptr<ThreeNode> root() const
  {
    return root_;
  }

  [[nodiscard]] std::shared_ptr<Blackboard> blackboard() const
  {
    return blackboard_;
  }

  // TODO: throw if no root; else return root_->executeTick()
  mini_bt::NodeStatus tickOnce()
  {
    throw std::runtime_error("TODO: Tree::tickOnce");
  }

  // TODO: loop executeTick until status != RUNNING (see l8_graduate)
  mini_bt::NodeStatus tickWhileRunning()
  {
    throw std::runtime_error("TODO: Tree::tickWhileRunning");
  }

private:
  std::shared_ptr<ThreeNode> root_;
  std::shared_ptr<Blackboard> blackboard_;
};
