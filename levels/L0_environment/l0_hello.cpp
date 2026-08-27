#include "mini_bt/types.hpp"

#include <iostream>

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  std::cout << "Hello MiniBT\n";
  std::cout << "NodeStatus values:\n";
  std::cout << "  " << toStr(NodeStatus::IDLE) << "\n";
  std::cout << "  " << toStr(NodeStatus::RUNNING) << "\n";
  std::cout << "  " << toStr(NodeStatus::SUCCESS) << "\n";
  std::cout << "  " << toStr(NodeStatus::FAILURE) << "\n";

  return 0;
}
