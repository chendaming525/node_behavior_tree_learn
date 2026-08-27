#pragma once

#include <tinyxml2.h>

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct XmlNode
{
  std::string tag;
  std::map<std::string, std::string> attributes;
  std::vector<XmlNode> children;
};

struct XmlDocumentModel
{
  XmlNode root;
  std::string btcpp_format;
  std::string main_tree_id;
  std::map<std::string, XmlNode> behavior_trees;
};

class XmlParser
{
public:
  XmlParser() = default;
  explicit XmlParser(std::string xml_file) : xml_file_(std::move(xml_file)) {}

  const XmlDocumentModel& parse()
  {
    if(xml_file_.empty())
    {
      throw std::runtime_error("parse() needs a file path; use parseText() for XML strings");
    }
    return parseFile(xml_file_);
  }

  const XmlDocumentModel& parseFile(const std::string& xml_file)
  {
    model_ = {};
    xml_file_ = xml_file;
    doc_.Clear();

    const tinyxml2::XMLError err = doc_.LoadFile(xml_file_.c_str());
    if(err != tinyxml2::XML_SUCCESS)
    {
      throw std::runtime_error(makeTinyXmlError("failed to load XML file: " + xml_file_));
    }

    return buildModel();
  }

  const XmlDocumentModel& parseText(const std::string& xml_text)
  {
    model_ = {};
    xml_file_.clear();
    doc_.Clear();

    const tinyxml2::XMLError err = doc_.Parse(xml_text.c_str(), xml_text.size());
    if(err != tinyxml2::XML_SUCCESS)
    {
      throw std::runtime_error(makeTinyXmlError("failed to parse XML text"));
    }

    return buildModel();
  }

  const XmlDocumentModel& model() const
  {
    return model_;
  }

  const XmlNode& mainTreeRoot() const
  {
    const auto it = model_.behavior_trees.find(model_.main_tree_id);
    if(it == model_.behavior_trees.end())
    {
      throw std::runtime_error("main tree is not parsed yet: " + model_.main_tree_id);
    }
    return it->second;
  }

  static void printNode(const XmlNode& node, std::ostream& os, int indent = 0)
  {
    const std::string spaces(static_cast<size_t>(indent), ' ');
    os << spaces << "<" << node.tag;
    for(const auto& [name, value] : node.attributes)
    {
      os << " " << name << "=\"" << value << "\"";
    }
    os << ">\n";

    for(const XmlNode& child : node.children)
    {
      printNode(child, os, indent + 2);
    }

    os << spaces << "</" << node.tag << ">\n";
  }

private:
  const XmlDocumentModel& buildModel()
  {
    const tinyxml2::XMLElement* root = doc_.RootElement();
    if(!root)
    {
      throw std::runtime_error("XML has no root element");
    }
    if(std::string(root->Name()) != "root")
    {
      throw std::runtime_error(makeElementError(*root, "expected <root> as XML root element"));
    }
    model_.root = parseElement(*root);

    if(const char* format = root->Attribute("BTCPP_format"))
    {
      model_.btcpp_format = format;
    }

    const char* main_tree = root->Attribute("main_tree_to_execute");
    if(!main_tree || std::string(main_tree).empty())
    {
      throw std::runtime_error(makeElementError(*root, "<root> missing attribute main_tree_to_execute"));
    }
    model_.main_tree_id = main_tree;

    for(const tinyxml2::XMLElement* tree = root->FirstChildElement("BehaviorTree");
        tree != nullptr;
        tree = tree->NextSiblingElement("BehaviorTree"))
    {
      const char* id = tree->Attribute("ID");
      if(!id || std::string(id).empty())
      {
        throw std::runtime_error(makeElementError(*tree, "<BehaviorTree> missing attribute ID"));
      }

      const std::string tree_id = id;
      if(model_.behavior_trees.find(tree_id) != model_.behavior_trees.end())
      {
        throw std::runtime_error(makeElementError(*tree, "duplicate BehaviorTree ID: " + tree_id));
      }

      const tinyxml2::XMLElement* tree_root = tree->FirstChildElement();
      if(!tree_root)
      {
        throw std::runtime_error(makeElementError(*tree, "BehaviorTree has no root node: " + tree_id));
      }

      model_.behavior_trees.emplace(tree_id, parseElement(*tree_root));
    }

    if(model_.behavior_trees.find(model_.main_tree_id) == model_.behavior_trees.end())
    {
      throw std::runtime_error("main_tree_to_execute not found: " + model_.main_tree_id);
    }

    return model_;
  }

  static XmlNode parseElement(const tinyxml2::XMLElement& element)
  {
    XmlNode node;
    node.tag = element.Name();

    for(const tinyxml2::XMLAttribute* attr = element.FirstAttribute();
        attr != nullptr;
        attr = attr->Next())
    {
      node.attributes.emplace(attr->Name(), attr->Value());
    }

    for(const tinyxml2::XMLElement* child = element.FirstChildElement();
        child != nullptr;
        child = child->NextSiblingElement())
    {
      node.children.push_back(parseElement(*child));
    }

    return node;
  }

  std::string makeTinyXmlError(const std::string& context) const
  {
    std::ostringstream os;
    os << context << ": " << doc_.ErrorStr();
    if(doc_.ErrorLineNum() > 0)
    {
      os << " at line " << doc_.ErrorLineNum();
    }
    return os.str();
  }

  static std::string makeElementError(const tinyxml2::XMLElement& element, const std::string& message)
  {
    std::ostringstream os;
    os << message << " at line " << element.GetLineNum();
    return os.str();
  }

  std::string xml_file_;
  tinyxml2::XMLDocument doc_;
  XmlDocumentModel model_;
};
