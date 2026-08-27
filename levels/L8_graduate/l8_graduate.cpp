#include "mini_bt/blackboard.hpp"
#include "mini_bt/enter_room_nodes.hpp"
#include "mini_bt/fallback.hpp"
#include "mini_bt/retry_node.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/sleep_action.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>

static NodeConfig makeConfig(std::shared_ptr<Blackboard> bb)
{
  NodeConfig config;
  config.blackboard = std::move(bb);
  return config;
}

static NodeConfig makeSetResultConfig(std::shared_ptr<Blackboard> bb)
{
  NodeConfig config = makeConfig(std::move(bb));
  config.input_ports["value"] = "entered room via key";
  config.output_ports["text"] = "{result}";
  return config;
}

static NodeConfig makeSayResultConfig(std::shared_ptr<Blackboard> bb)
{
  NodeConfig config = makeConfig(std::move(bb));
  config.input_ports["message"] = "{result}";
  return config;
}

/**
 * L8 graduation demo — "enter the room"
 *
 * Fallback
 *   Sequence plan_A: BatteryOK → DoorOpen(fail) → EnterRoom
 *   Sequence plan_B: FindKey → Retry(FlakyOpenDoor) → Sleep(2) → EnterRoom
 *                    → SetBlackboard(result)
 * Sequence root: Fallback → SaySomething(result)
 *
 * Expected: plan A fails at DoorOpen; plan B retries open, sleeps, enters; say hello.
 */
static void tickWhileRunning(ThreeNode& root)
{
  using mini_bt::NodeStatus;
  int n = 0;
  NodeStatus st = NodeStatus::IDLE;
  do
  {
    ++n;
    st = root.executeTick();
    std::cout << "  [main] tick#" << n << " root -> " << mini_bt::toStr(st) << "\n";
  } while(st == NodeStatus::RUNNING);
}

int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto bb = std::make_shared<Blackboard>();

  // --- plan A: door already open? (fails) ---
  auto plan_a = std::make_shared<SequenceNode>("plan_A", makeConfig(bb));
  plan_a->addChild(std::make_shared<BatteryOK>("BatteryOK", makeConfig(bb)));
  plan_a->addChild(std::make_shared<DoorOpen>("DoorOpen", makeConfig(bb)));
  plan_a->addChild(std::make_shared<EnterRoom>("EnterRoom_A", makeConfig(bb)));

  // --- plan B: find key, retry open, brief wait, enter ---
  auto open_door = std::make_shared<FlakyOpenDoor>("OpenDoor", makeConfig(bb), 2);
  auto retry_open =
      std::make_shared<RetryNode>("RetryOpen", makeConfig(bb), open_door, /*max attempts*/ 3);

  auto plan_b = std::make_shared<SequenceNode>("plan_B", makeConfig(bb));
  plan_b->addChild(std::make_shared<FindKey>("FindKey", makeConfig(bb)));
  plan_b->addChild(retry_open);
  plan_b->addChild(std::make_shared<SleepAction>("WaitAfterOpen", makeConfig(bb), 2));
  plan_b->addChild(std::make_shared<EnterRoom>("EnterRoom_B", makeConfig(bb)));
  plan_b->addChild(std::make_shared<SetBlackboardNode>("SetResult", makeSetResultConfig(bb)));

  auto enter = std::make_shared<FallbackNode>("EnterRoomFallback", makeConfig(bb));
  enter->addChild(plan_a);
  enter->addChild(plan_b);

  auto root = std::make_shared<SequenceNode>("MainTree", makeConfig(bb));
  root->addChild(enter);
  root->addChild(std::make_shared<SaySomethingNode>("SayResult", makeSayResultConfig(bb)));

  std::cout << "========== L8 enter-room demo ==========\n";
  tickWhileRunning(*root);
  std::cout << "========== done: root status "
            << toStr(root->curStatus()) << " ==========\n";

  return root->curStatus() == NodeStatus::SUCCESS ? 0 : 1;
}
