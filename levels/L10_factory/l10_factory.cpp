#include "bt_factory.hpp"
#include "enter_room_nodes.hpp"
#include "sequence.hpp"

#include <memory>

int main()
{
  auto bb = std::make_shared<Blackboard>();
  NodeFactory factory;
  factory.registerNodeType<BatteryOK>("BatteryOK");
  factory.registerNodeType<EnterRoom>("EnterRoom");

  NodeConfig config;
  config.blackboard = bb;
  config.input_ports = {{"value", "hello"}};
  config.output_ports = {{"text", "{door_message}"}};

  // 节点
  auto batteryOKNode = factory.instantiateNode("BatteryOK", "BatteryOK_Node", config);
  auto enterRoomNode = factory.instantiateNode("EnterRoom", "EnterRoom_Node", config);
  SequenceNode sequenceNode("Sequence_Node", config);
  sequenceNode.addChild(batteryOKNode);
  sequenceNode.addChild(enterRoomNode);
  sequenceNode.tick();
  // 节点的配置
  // 创建节点

  return 0;
}
