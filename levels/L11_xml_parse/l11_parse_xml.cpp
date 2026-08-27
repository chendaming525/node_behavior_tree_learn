#include "mini_bt/xml_parser.hpp"

#include <iostream>

int main()
{
  XmlParser parser(L11_ENTER_ROOM_XML);
  const XmlDocumentModel& model = parser.parse();

  std::cout << "========== parse from file ==========\n";
  std::cout << "BTCPP_format = " << model.btcpp_format << "\n";
  std::cout << "main_tree_to_execute = " << model.main_tree_id << "\n";
  XmlParser::printNode(model.root, std::cout);

  std::cout << "========== parse from text ==========\n";
  XmlParser text_parser;
  const XmlDocumentModel& text_model = text_parser.parseText(R"(
<root BTCPP_format="4" main_tree_to_execute="MainTree">
  <BehaviorTree ID="MainTree">
    <Sequence name="main">
      <SaySomething name="say" message="{result}"/>
    </Sequence>
  </BehaviorTree>
</root>
)");

  std::cout << "main tree root tag = " << text_parser.mainTreeRoot().tag << "\n";
  std::cout << "text main_tree_to_execute = " << text_model.main_tree_id << "\n";

  return 0;
}
