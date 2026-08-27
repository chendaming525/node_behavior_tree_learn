#pragma once

#include "mini_bt/bt_factory.hpp"
#include "mini_bt/enter_room_nodes.hpp"
#include "mini_bt/fallback.hpp"
#include "mini_bt/sequence.hpp"

inline void registerBuiltinNodes(NodeFactory& factory)
{
  factory.registerNodeType<SequenceNode>("Sequence");
  factory.registerNodeType<FallbackNode>("Fallback");

  factory.registerNodeType<BatteryOK>("BatteryOK");
  factory.registerNodeType<DoorOpen>("DoorOpen");
  factory.registerNodeType<FindKey>("FindKey");
  factory.registerNodeType<EnterRoom>("EnterRoom");

  factory.registerNodeType<SetBlackboardNode>("SetBlackboard");
  factory.registerNodeType<SaySomethingNode>("SaySomething");
}
