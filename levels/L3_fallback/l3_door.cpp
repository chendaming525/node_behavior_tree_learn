#include "mini_bt/fallback.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/three_node.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

class BatteryOK : public ThreeNode
{
public:
  explicit BatteryOK(std::string name) : ThreeNode(std::move(name))
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "[" << getName() << "] battery OK\n";
    return mini_bt::NodeStatus::SUCCESS;
  }
};

class DoorOpen : public ThreeNode
{
public:
  explicit DoorOpen(std::string name) : ThreeNode(std::move(name))
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "[" << getName() << "] door is CLOSED\n";
    return mini_bt::NodeStatus::FAILURE;
  }
};

class FindKey : public ThreeNode
{
public:
  explicit FindKey(std::string name) : ThreeNode(std::move(name))
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "[" << getName() << "] found key\n";
    return mini_bt::NodeStatus::SUCCESS;
  }
};

class OpenDoor : public ThreeNode
{
public:
  explicit OpenDoor(std::string name) : ThreeNode(std::move(name))
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "[" << getName() << "] opened door\n";
    return mini_bt::NodeStatus::SUCCESS;
  }
};

class EnterRoom : public ThreeNode
{
public:
  explicit EnterRoom(std::string name) : ThreeNode(std::move(name))
  {}

protected:
  mini_bt::NodeStatus tick() override
  {
    std::cout << "[" << getName() << "] entered room\n";
    return mini_bt::NodeStatus::SUCCESS;
  }
};

int main()
{
  using mini_bt::toStr;

  auto plan_a = std::make_shared<SequenceNode>("plan_A");
  plan_a->addChild(std::make_shared<BatteryOK>("BatteryOK"));
  plan_a->addChild(std::make_shared<DoorOpen>("DoorOpen"));
  plan_a->addChild(std::make_shared<EnterRoom>("EnterRoom_A"));

  auto plan_b = std::make_shared<SequenceNode>("plan_B");
  plan_b->addChild(std::make_shared<FindKey>("FindKey"));
  plan_b->addChild(std::make_shared<OpenDoor>("OpenDoor"));
  plan_b->addChild(std::make_shared<EnterRoom>("EnterRoom_B"));

  auto root = std::make_shared<FallbackNode>("EnterRoomFallback");
  root->addChild(plan_a);
  root->addChild(plan_b);

  const auto status = root->executeTick();
  std::cout << "root -> " << toStr(status) << "\n";
  return status == mini_bt::NodeStatus::SUCCESS ? 0 : 1;
}
