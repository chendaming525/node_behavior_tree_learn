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

int main()
{
  XmlParser parser(L13_SUBTREE_XML);
  const XmlDocumentModel& model = parser.parse();

  FactoryXmlTreeBuilder builder;
  const auto root = builder.buildTree(model);

  std::cout << "========== L13 SubTree demo ==========\n";
  const mini_bt::NodeStatus status = tickWhileRunning(*root);
  std::cout << "========== done: root status " << mini_bt::toStr(status) << " ==========\n";

  return status == mini_bt::NodeStatus::SUCCESS ? 0 : 1;
}
