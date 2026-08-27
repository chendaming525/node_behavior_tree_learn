#include "mini_bt/retry_node.hpp"
#include "mini_bt/three_node.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>
#include <utility>

class FlakyLeaf : public ThreeNode
{
public:
  FlakyLeaf(std::string name, int failures_before_success)
    : ThreeNode(std::move(name)), failures_before_success_(failures_before_success)
  {}

  mini_bt::NodeStatus tick() override
  {
    ++tries_;
    if(tries_ <= failures_before_success_)
    {
      std::cout << "[" << getName() << "] fail try " << tries_ << "\n";
      return mini_bt::NodeStatus::FAILURE;
    }
    std::cout << "[" << getName() << "] success try " << tries_ << "\n";
    return mini_bt::NodeStatus::SUCCESS;
  }

private:
  int failures_before_success_ = 2;
  int tries_ = 0;
};

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto flaky = std::make_shared<FlakyLeaf>("flaky", 2);
  auto root = std::make_shared<RetryNode>("retry", flaky, 3);

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
