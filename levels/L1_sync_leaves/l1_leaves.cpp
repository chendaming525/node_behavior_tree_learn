#include "mini_bt/action_node.hpp"
#include "mini_bt/types.hpp"

#include <iostream>

int main()
{
  AlwaysSuccessNode ok("always_ok");
  AlwaysFailureNode bad("always_bad");

  const mini_bt::NodeStatus s1 = ok.executeTick();
  const mini_bt::NodeStatus s2 = bad.executeTick();

  std::cout << "[" << ok.getName() << "] -> " << mini_bt::toStr(s1) << "\n";
  std::cout << "[" << bad.getName() << "] -> " << mini_bt::toStr(s2) << "\n";

  return (s1 == mini_bt::NodeStatus::SUCCESS && s2 == mini_bt::NodeStatus::FAILURE) ? 0
                                                                                   : 1;
}
