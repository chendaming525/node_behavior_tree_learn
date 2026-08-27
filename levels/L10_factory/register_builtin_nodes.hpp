#pragma once

// =============================================================================
// register_builtin_nodes.hpp — L10：批量注册常用节点类型
// =============================================================================

#include "action_node.hpp"
#include "bt_factory.hpp"
#include "enter_room_nodes.hpp"
#include "fallback.hpp"
#include "sequence.hpp"
#include "sleep_action.hpp"

// Suggested registrations (IDs should match future XML tags):
//
// Controls:
//   "Sequence" -> SequenceNode
//   "Fallback" -> FallbackNode
//
// Leaves without ports:
//   "AlwaysSuccess" / "AlwaysFailure"
//   "BatteryOK" / "DoorOpen" / "FindKey" / "EnterRoom"
//   "Sleep" -> SleepAction
//
// Leaves with ports:
//   "SetBlackboard" -> SetBlackboardNode
//   "SaySomething"  -> SaySomethingNode
//
// Skip for now:
//   "Retry" -> RetryNode（ctor 需要 child，NodeBuilder(name, config) 不够）

inline void registerBuiltinNodes(NodeFactory& factory)
{
  factory.registerNodeType<SequenceNode>("Sequence");
  factory.registerNodeType<FallbackNode>("Fallback");

  factory.registerNodeType<AlwaysSuccessNode>("AlwaysSuccess");
  factory.registerNodeType<AlwaysFailureNode>("AlwaysFailure");

  factory.registerNodeType<BatteryOK>("BatteryOK");
  factory.registerNodeType<DoorOpen>("DoorOpen");
  factory.registerNodeType<FindKey>("FindKey");
  factory.registerNodeType<EnterRoom>("EnterRoom");
  factory.registerNodeType<SleepAction>("Sleep");

  factory.registerNodeType<SetBlackboardNode>("SetBlackboard");
  factory.registerNodeType<SaySomethingNode>("SaySomething");
}
