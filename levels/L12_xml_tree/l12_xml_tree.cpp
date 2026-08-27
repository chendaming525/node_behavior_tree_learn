#include "mini_bt/xml_instantiation.hpp"

#include <iostream>

static mini_bt::NodeStatus tickWhileRunning(ThreeNode& root)
{
  mini_bt::NodeStatus status = mini_bt::NodeStatus::IDLE;
  int tick_count = 0;

  do
  {
    ++tick_count;
    status = root.executeTick();
    std::cout << "[main] tick#" << tick_count << " root -> " << mini_bt::toStr(status) << "\n";
  } while(status == mini_bt::NodeStatus::RUNNING);

  return status;
}

static int runManualBuilder(const XmlDocumentModel& model)
{
  XmlTreeBuilder builder;
  const auto root = builder.buildTree(model);

  std::cout << "========== L12 manual XML tree demo ==========\n";
  const mini_bt::NodeStatus status = tickWhileRunning(*root);
  std::cout << "========== manual done: root status " << mini_bt::toStr(status) << " ==========\n";

  return status == mini_bt::NodeStatus::SUCCESS ? 0 : 1;
}

static int runFactoryBuilder(const XmlDocumentModel& model)
{
  FactoryXmlTreeBuilder builder;
  const auto root = builder.buildTree(model);

  std::cout << "========== L12 factory XML tree demo ==========\n";
  const mini_bt::NodeStatus status = tickWhileRunning(*root);
  std::cout << "========== factory done: root status " << mini_bt::toStr(status) << " ==========\n";

  return status == mini_bt::NodeStatus::SUCCESS ? 0 : 1;
}

int main()
{
  XmlParser parser(L12_ENTER_ROOM_XML);
  const XmlDocumentModel& model = parser.parse();

  const int manual_result = runManualBuilder(model);
  const int factory_result = runFactoryBuilder(model);
  return manual_result == 0 && factory_result == 0 ? 0 : 1;
}
