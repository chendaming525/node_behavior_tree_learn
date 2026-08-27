#include "mini_bt/action_node.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/sleep_action.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto root = std::make_shared<SequenceNode>("root");
  root->addChild(std::make_shared<AlwaysSuccessNode>("before"));
  root->addChild(std::make_shared<SleepAction>("sleep", 3));
  root->addChild(std::make_shared<AlwaysSuccessNode>("after"));

  int n = 0;
  NodeStatus st = NodeStatus::IDLE;
  do
  {
    ++n;
    st = root->executeTick();
    std::cout << "tick#" << n << " root -> " << toStr(st) << "\n";
  } while(st == NodeStatus::RUNNING);

  return st == NodeStatus::SUCCESS ? 0 : 1;
}
