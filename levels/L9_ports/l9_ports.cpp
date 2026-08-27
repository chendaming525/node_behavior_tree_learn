#include "mini_bt/blackboard.hpp"
#include "mini_bt/enter_room_nodes.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/types.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

static NodeConfig withBlackboard(std::shared_ptr<Blackboard> bb)
{
  NodeConfig config;
  config.blackboard = std::move(bb);
  return config;
}

static NodeConfig writerConfig(
    std::shared_ptr<Blackboard> bb, std::string value, std::string output_ref)
{
  NodeConfig config = withBlackboard(std::move(bb));
  config.input_ports["value"] = std::move(value);
  config.output_ports["text"] = std::move(output_ref);
  return config;
}

static NodeConfig sayConfig(std::shared_ptr<Blackboard> bb, std::string message)
{
  NodeConfig config = withBlackboard(std::move(bb));
  config.input_ports["message"] = std::move(message);
  return config;
}

static void section(const std::string& title)
{
  std::cout << "\n========== " << title << " ==========\n";
}

/**
 * L9 demo - Port remapping
 *
 * Shows three valid usages with the SAME node classes:
 *   1) producer output {key} -> consumer input {key}
 *   2) constant input (no blackboard read)
 *   3) same SaySomething remapped to a different blackboard key
 *
 * Plus a few intentional config errors that must fail clearly.
 */
int main()
{
  using mini_bt::NodeStatus;
  using mini_bt::toStr;

  auto bb = std::make_shared<Blackboard>();

  // -------------------------------------------------------------------------
  // Usage 1 + 2: write {door_message}, read {door_message}, then constant
  // -------------------------------------------------------------------------
  section("usage 1+2: remapping + constant");

  auto demo12 = std::make_shared<SequenceNode>("Demo12", withBlackboard(bb));
  demo12->addChild(std::make_shared<SetBlackboardNode>(
      "WriteDoorMessage",
      writerConfig(bb, "door opened through port remapping", "{door_message}")));
  demo12->addChild(std::make_shared<SaySomethingNode>(
      "SayFromBlackboard", sayConfig(bb, "{door_message}")));
  demo12->addChild(std::make_shared<SaySomethingNode>(
      "SayConstant", sayConfig(bb, "hello from a constant input port")));

  NodeStatus status = demo12->executeTick();
  std::cout << "Demo12 root -> " << toStr(status) << "\n";
  if(status != NodeStatus::SUCCESS)
  {
    return 1;
  }

  // -------------------------------------------------------------------------
  // Usage 3: same SaySomething class, different remapping, zero node-code change
  // -------------------------------------------------------------------------
  section("usage 3: same node, different blackboard key");

  // Pretend another producer wrote a different slot.
  bb->set(std::string("error_text"), std::string("battery low"));

  SaySomethingNode say_error("SayError", sayConfig(bb, "{error_text}"));
  status = say_error.executeTick();
  std::cout << "SayError -> " << toStr(status) << "\n";
  if(status != NodeStatus::SUCCESS)
  {
    return 1;
  }

  // -------------------------------------------------------------------------
  // Expected failures: prove remapping/config is validated
  // -------------------------------------------------------------------------
  section("expected config errors");

  try
  {
    SetBlackboardNode bad_output(
        "BadOutput", writerConfig(bb, "this should fail", "not_a_blackboard_ref"));
    bad_output.executeTick();
    std::cout << "ERROR: BadOutput should have thrown\n";
    return 1;
  }
  catch(const std::exception& ex)
  {
    std::cout << "[ok] output must be {key}: " << ex.what() << "\n";
  }

  try
  {
    NodeConfig missing_output = withBlackboard(bb);
    missing_output.input_ports["value"] = "x";
    // intentionally no output_ports["text"]
    SetBlackboardNode missing("MissingOutputRemap", std::move(missing_output));
    missing.executeTick();
    std::cout << "ERROR: MissingOutputRemap should have thrown\n";
    return 1;
  }
  catch(const std::exception& ex)
  {
    std::cout << "[ok] missing output remapping: " << ex.what() << "\n";
  }

  {
    SaySomethingNode missing_input("MissingInputRemap", withBlackboard(bb));
    status = missing_input.executeTick();
    std::cout << "[ok] missing input remapping -> " << toStr(status) << "\n";
    if(status != NodeStatus::FAILURE)
    {
      return 1;
    }
  }

  try
  {
    // Wrong port name inside node code path: getInput("msgge")
    class WrongPortName : public ThreeNode
    {
    public:
      WrongPortName(std::string name, NodeConfig config)
        : ThreeNode(std::move(name), std::move(config))
      {}

      static PortsList providedPorts()
      {
        return {InputPort<std::string>("message")};
      }

      PortsList ports() const override
      {
        return providedPorts();
      }

      NodeStatus tick() override
      {
        // typo on purpose: should throw "unknown port msgge"
        (void)getInput<std::string>("msgge");
        return NodeStatus::SUCCESS;
      }
    };

    WrongPortName wrong("WrongPort", sayConfig(bb, "hello"));
    wrong.executeTick();
    std::cout << "ERROR: WrongPort should have thrown\n";
    return 1;
  }
  catch(const std::exception& ex)
  {
    std::cout << "[ok] unknown port name: " << ex.what() << "\n";
  }

  section("L9 ports demo PASSED");
  return 0;
}
