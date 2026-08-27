#include "mini_bt/action_node.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>

class LoggingLeaf : public ThreeNode
{
public:
  LoggingLeaf(std::string name, mini_bt::NodeStatus result)
    : ThreeNode(std::move(name)), result_(result)
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "  tick [" << getName() << "]\n";
    return result_;
  }

private:
  mini_bt::NodeStatus result_;
};

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  std::cout << "=== Demo A: all success ===\n";
  auto a = std::make_shared<SequenceNode>("seq_a");
  a->addChild(std::make_shared<LoggingLeaf>("a1", NodeStatus::SUCCESS));
  a->addChild(std::make_shared<LoggingLeaf>("a2", NodeStatus::SUCCESS));
  a->addChild(std::make_shared<LoggingLeaf>("a3", NodeStatus::SUCCESS));
  std::cout << "root -> " << toStr(a->executeTick()) << "\n";

  std::cout << "=== Demo B: fail in the middle (c3 must not tick) ===\n";
  auto b = std::make_shared<SequenceNode>("seq_b");
  b->addChild(std::make_shared<LoggingLeaf>("b1", NodeStatus::SUCCESS));
  b->addChild(std::make_shared<LoggingLeaf>("b2", NodeStatus::FAILURE));
  b->addChild(std::make_shared<LoggingLeaf>("b3", NodeStatus::SUCCESS));
  std::cout << "root -> " << toStr(b->executeTick()) << "\n";

  return 0;
}
