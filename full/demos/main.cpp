#include "mini_bt/xml_instantiation.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

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
  try
  {
    XmlParser parser(FULL_ENTER_ROOM_XML);
    const XmlDocumentModel& model = parser.parse();

    FactoryXmlTreeBuilder builder;
    const auto root = builder.buildTree(model);

    std::cout << "========== full XML graduate ==========\n";
    const mini_bt::NodeStatus status = tickWhileRunning(*root);

    const std::string result = builder.blackboard()->get<std::string>("final_result");
    std::cout << "[check] final_result = " << result << "\n";
    std::cout << "========== done: root status " << mini_bt::toStr(status) << " ==========\n";

    if(status != mini_bt::NodeStatus::SUCCESS)
    {
      return 1;
    }
    return result == "entered room via subtree" ? 0 : 1;
  }
  catch(const std::exception& ex)
  {
    std::cerr << "full XML graduate failed: " << ex.what() << "\n";
    return 1;
  }
}
