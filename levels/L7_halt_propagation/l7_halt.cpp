#include "mini_bt/action_node.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/sleep_action.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>

/** L7-style check: Sequence halt propagates to Sleep → onHalted */
int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto sleep = std::make_shared<SleepAction>("sleep");
  auto root = std::make_shared<SequenceNode>("root");
  root->addChild(sleep);
  root->addChild(std::make_shared<AlwaysSuccessNode>("after"));

  std::cout << "--- tick until Sleep RUNNING ---\n";
  NodeStatus st = root->executeTick();
  std::cout << "root -> " << toStr(st) << "\n";

  std::cout << "--- halt root (should call Sleep onHalted) ---\n";
  root->halt();
  std::cout << "sleep status -> " << toStr(sleep->curStatus()) << "\n";

  std::cout << "--- tick again (Sleep should onStart again) ---\n";
  st = root->executeTick();
  std::cout << "root -> " << toStr(st) << "\n";

  return (sleep->curStatus() == NodeStatus::RUNNING || st == NodeStatus::RUNNING) ? 0 : 1;
}
