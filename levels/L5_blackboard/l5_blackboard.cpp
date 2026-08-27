#include "mini_bt/sequence.hpp"
#include "mini_bt/three_node.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

class SetBlackboardNode : public ThreeNode
{
public:
  SetBlackboardNode(std::string name, std::string key, std::string value)
    : ThreeNode(std::move(name)), key_(std::move(key)), value_(std::move(value))
  {}

  mini_bt::NodeStatus tick() override
  {
    setOutput(key_, value_);
    std::cout << "[" << getName() << "] blackboard[" << key_ << "] = \"" << value_
              << "\"\n";
    return mini_bt::NodeStatus::SUCCESS;
  }

private:
  std::string key_;
  std::string value_;
};

class SaySomethingNode : public ThreeNode
{
public:
  SaySomethingNode(std::string name, std::string key)
    : ThreeNode(std::move(name)), key_(std::move(key))
  {}

  mini_bt::NodeStatus tick() override
  {
    std::string msg;
    if(!getInput(key_, msg))
    {
      std::cout << "[" << getName() << "] missing key: " << key_ << "\n";
      return mini_bt::NodeStatus::FAILURE;
    }
    std::cout << "[" << getName() << "] Robot says: " << msg << "\n";
    return mini_bt::NodeStatus::SUCCESS;
  }

private:
  std::string key_;
};

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto bb = std::make_shared<Blackboard>();
  auto root = std::make_shared<SequenceNode>("root");
  root->setBlackboard(bb);
  root->addChild(std::make_shared<SetBlackboardNode>("Set", "msg", "hello"));
  root->addChild(std::make_shared<SaySomethingNode>("Say", "msg"));

  const NodeStatus st = root->executeTick();
  std::cout << "root -> " << toStr(st) << "\n";
  return st == NodeStatus::SUCCESS ? 0 : 1;
}
