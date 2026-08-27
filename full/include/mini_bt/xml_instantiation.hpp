#pragma once

#include "mini_bt/blackboard.hpp"
#include "mini_bt/bt_factory.hpp"
#include "mini_bt/control_node.hpp"
#include "mini_bt/enter_room_nodes.hpp"
#include "mini_bt/fallback.hpp"
#include "mini_bt/ports.hpp"
#include "mini_bt/register_builtin_nodes.hpp"
#include "mini_bt/retry_node.hpp"
#include "mini_bt/sequence.hpp"
#include "mini_bt/sleep_action.hpp"
#include "mini_bt/three_node.hpp"
#include "mini_bt/types.hpp"
#include "mini_bt/xml_parser.hpp"

#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace xml_tree_detail
{
inline std::string getAttributeOr(
    const XmlNode& xml_node,
    const std::string& name,
    const std::string& default_value)
{
  const auto it = xml_node.attributes.find(name);
  if(it == xml_node.attributes.end())
  {
    return default_value;
  }
  return it->second;
}

inline std::string getRequiredAttribute(const XmlNode& xml_node, const std::string& name)
{
  const auto it = xml_node.attributes.find(name);
  if(it == xml_node.attributes.end() || it->second.empty())
  {
    throw std::runtime_error(xml_node.tag + " missing required attribute: " + name);
  }
  return it->second;
}

inline int getIntAttribute(const XmlNode& xml_node, const std::string& name)
{
  const std::string value = getRequiredAttribute(xml_node, name);
  char* end = nullptr;
  const long parsed = std::strtol(value.c_str(), &end, 10);
  if(end == value.c_str() || *end != '\0')
  {
    throw std::runtime_error(xml_node.tag + " attribute is not an integer: " + name + "=" + value);
  }
  return static_cast<int>(parsed);
}

inline void ensureLeaf(const XmlNode& xml_node)
{
  if(!xml_node.children.empty())
  {
    throw std::runtime_error("leaf node must not have children: " + xml_node.tag);
  }
}

inline void requireChildCount(const XmlNode& xml_node, size_t expected)
{
  if(xml_node.children.size() != expected)
  {
    throw std::runtime_error(xml_node.tag + " expects " + std::to_string(expected)
                             + " child node(s), got "
                             + std::to_string(xml_node.children.size()));
  }
}
}   // namespace xml_tree_detail

class SubTreeNode : public ThreeNode
{
public:
  SubTreeNode(std::string name, NodeConfig config, std::shared_ptr<ThreeNode> child)
    : ThreeNode(std::move(name), std::move(config)), child_(std::move(child))
  {}

  mini_bt::NodeStatus tick() override
  {
    return child_->executeTick();
  }

  void halt() override
  {
    child_->halt();
    ThreeNode::halt();
  }

  void setBlackboard(std::shared_ptr<Blackboard> bb) override
  {
    // Keep the child on its scoped SubTree blackboard. The wrapper itself can
    // receive the parent blackboard when it is attached to a control node.
    ThreeNode::setBlackboard(std::move(bb));
  }

private:
  std::shared_ptr<ThreeNode> child_;
};

class XmlTreeBuilder
{
public:
  XmlTreeBuilder() : blackboard_(std::make_shared<Blackboard>()) {}

  explicit XmlTreeBuilder(std::shared_ptr<Blackboard> blackboard)
    : blackboard_(std::move(blackboard))
  {
    if(!blackboard_)
    {
      blackboard_ = std::make_shared<Blackboard>();
    }
  }

  std::shared_ptr<ThreeNode> buildTree(const XmlDocumentModel& model)
  {
    const auto it = model.behavior_trees.find(model.main_tree_id);
    if(it == model.behavior_trees.end())
    {
      throw std::runtime_error("main tree not found: " + model.main_tree_id);
    }
    return buildNode(it->second);
  }

  std::shared_ptr<ThreeNode> buildNode(const XmlNode& xml_node)
  {
    const std::string& type = xml_node.tag;
    const std::string name = xml_tree_detail::getAttributeOr(xml_node, "name", type);
    NodeConfig config = makeConfig();

    if(type == "Sequence")
    {
      auto node = std::make_shared<SequenceNode>(name, config);
      attachControlChildren(*node, xml_node);
      return node;
    }

    if(type == "Fallback")
    {
      auto node = std::make_shared<FallbackNode>(name, config);
      attachControlChildren(*node, xml_node);
      return node;
    }

    if(type == "Retry")
    {
      xml_tree_detail::requireChildCount(xml_node, 1);
      const int attempts = xml_tree_detail::getIntAttribute(xml_node, "num_attempts");
      return std::make_shared<RetryNode>(name, config, buildNode(xml_node.children.front()), attempts);
    }

    xml_tree_detail::ensureLeaf(xml_node);

    if(type == "BatteryOK")
    {
      return std::make_shared<BatteryOK>(name, config);
    }

    if(type == "DoorOpen")
    {
      return std::make_shared<DoorOpen>(name, config);
    }

    if(type == "FindKey")
    {
      return std::make_shared<FindKey>(name, config);
    }

    if(type == "FlakyOpenDoor")
    {
      const int failures = xml_tree_detail::getIntAttribute(xml_node, "failures_before_success");
      return std::make_shared<FlakyOpenDoor>(name, config, failures);
    }

    if(type == "Sleep")
    {
      const int ticks = xml_tree_detail::getIntAttribute(xml_node, "ticks");
      return std::make_shared<SleepAction>(name, config, ticks);
    }

    if(type == "EnterRoom")
    {
      return std::make_shared<EnterRoom>(name, config);
    }

    if(type == "SetBlackboard")
    {
      config.input_ports["value"] = xml_tree_detail::getRequiredAttribute(xml_node, "value");
      config.output_ports["text"] = xml_tree_detail::getRequiredAttribute(xml_node, "text");
      return std::make_shared<SetBlackboardNode>(name, config);
    }

    if(type == "SaySomething")
    {
      config.input_ports["message"] = xml_tree_detail::getRequiredAttribute(xml_node, "message");
      return std::make_shared<SaySomethingNode>(name, config);
    }

    throw std::runtime_error("unknown XML node tag: " + type);
  }

  std::shared_ptr<Blackboard> blackboard() const
  {
    return blackboard_;
  }

private:
  NodeConfig makeConfig() const
  {
    NodeConfig config;
    config.blackboard = blackboard_;
    return config;
  }

  void attachControlChildren(ControlNode& node, const XmlNode& xml_node)
  {
    for(const XmlNode& child_xml : xml_node.children)
    {
      node.addChild(buildNode(child_xml));
    }
  }

  std::shared_ptr<Blackboard> blackboard_;
};

class FactoryXmlTreeBuilder
{
public:
  FactoryXmlTreeBuilder()
    : blackboard_(std::make_shared<Blackboard>())
  {
    registerBuiltinNodes(factory_);
  }

  explicit FactoryXmlTreeBuilder(std::shared_ptr<Blackboard> blackboard)
    : blackboard_(std::move(blackboard))
  {
    if(!blackboard_)
    {
      blackboard_ = std::make_shared<Blackboard>();
    }
    registerBuiltinNodes(factory_);
  }

  std::shared_ptr<ThreeNode> buildTree(const XmlDocumentModel& model)
  {
    model_ = &model;
    subtree_stack_.clear();

    const auto it = model.behavior_trees.find(model.main_tree_id);
    if(it == model.behavior_trees.end())
    {
      throw std::runtime_error("main tree not found: " + model.main_tree_id);
    }
    return buildNode(it->second);
  }

  std::shared_ptr<ThreeNode> buildNode(const XmlNode& xml_node)
  {
    const std::string& type = xml_node.tag;
    const std::string name = xml_tree_detail::getAttributeOr(xml_node, "name", type);

    if(type == "SubTree")
    {
      return buildSubTree(xml_node);
    }

    if(type == "Retry")
    {
      xml_tree_detail::requireChildCount(xml_node, 1);
      const int attempts = xml_tree_detail::getIntAttribute(xml_node, "num_attempts");
      return std::make_shared<RetryNode>(name, makeConfig(), buildNode(xml_node.children.front()), attempts);
    }

    if(type == "Sleep")
    {
      xml_tree_detail::ensureLeaf(xml_node);
      const int ticks = xml_tree_detail::getIntAttribute(xml_node, "ticks");
      return std::make_shared<SleepAction>(name, makeConfig(), ticks);
    }

    if(type == "FlakyOpenDoor")
    {
      xml_tree_detail::ensureLeaf(xml_node);
      const int failures = xml_tree_detail::getIntAttribute(xml_node, "failures_before_success");
      return std::make_shared<FlakyOpenDoor>(name, makeConfig(), failures);
    }

    if(!factory_.isRegistered(type))
    {
      throw std::runtime_error("unknown XML node tag: " + type);
    }

    NodeConfig config = makeConfigFromManifest(xml_node);
    auto node = factory_.instantiateNode(type, name, config);

    if(type == "Sequence" || type == "Fallback")
    {
      auto control = std::dynamic_pointer_cast<ControlNode>(node);
      if(!control)
      {
        throw std::runtime_error(type + " was registered but is not a ControlNode");
      }
      for(const XmlNode& child_xml : xml_node.children)
      {
        control->addChild(buildNode(child_xml));
      }
      return node;
    }

    xml_tree_detail::ensureLeaf(xml_node);
    return node;
  }

  std::shared_ptr<Blackboard> blackboard() const
  {
    return blackboard_;
  }

private:
  NodeConfig makeConfig() const
  {
    NodeConfig config;
    config.blackboard = blackboard_;
    return config;
  }

  NodeConfig makeConfigFromManifest(const XmlNode& xml_node) const
  {
    NodeConfig config = makeConfig();
    const PortsList ports = factory_.getPortsList(xml_node.tag);

    for(const auto& [name, value] : xml_node.attributes)
    {
      if(name == "name")
      {
        continue;
      }

      const auto port_it = ports.find(name);
      if(port_it == ports.end())
      {
        throw std::runtime_error(xml_node.tag + " has unknown XML attribute: " + name);
      }

      if(port_it->second.direction == PortDirection::INPUT)
      {
        config.input_ports[name] = value;
      }
      else
      {
        config.output_ports[name] = value;
      }
    }

    for(const auto& [name, info] : ports)
    {
      if(info.direction == PortDirection::INPUT && config.input_ports.count(name) == 0)
      {
        throw std::runtime_error(xml_node.tag + " missing input port attribute: " + name);
      }
      if(info.direction == PortDirection::OUTPUT && config.output_ports.count(name) == 0)
      {
        throw std::runtime_error(xml_node.tag + " missing output port attribute: " + name);
      }
    }

    return config;
  }

  std::shared_ptr<ThreeNode> buildSubTree(const XmlNode& xml_node)
  {
    if(!model_)
    {
      throw std::runtime_error("SubTree needs an XmlDocumentModel");
    }

    xml_tree_detail::ensureLeaf(xml_node);
    const std::string tree_id = xml_tree_detail::getRequiredAttribute(xml_node, "ID");
    const std::string name = xml_tree_detail::getAttributeOr(xml_node, "name", "SubTree:" + tree_id);
    const auto it = model_->behavior_trees.find(tree_id);
    if(it == model_->behavior_trees.end())
    {
      throw std::runtime_error("SubTree target not found: " + tree_id);
    }

    for(const std::string& active_id : subtree_stack_)
    {
      if(active_id == tree_id)
      {
        throw std::runtime_error("recursive SubTree reference: " + tree_id);
      }
    }

    std::map<std::string, std::string> remapping;
    for(const auto& [name, value] : xml_node.attributes)
    {
      if(name == "ID" || name == "name")
      {
        continue;
      }
      if(!isBlackboardPointer(value))
      {
        throw std::runtime_error("SubTree remapping must use {parent_key}: " + name + "=" + value);
      }
      remapping[name] = stripBlackboardPointer(value);
    }

    auto parent_blackboard = blackboard_;
    auto child_blackboard = std::make_shared<Blackboard>(parent_blackboard, remapping);

    subtree_stack_.push_back(tree_id);
    blackboard_ = child_blackboard;
    auto root = buildNode(it->second);
    blackboard_ = parent_blackboard;
    subtree_stack_.pop_back();

    return std::make_shared<SubTreeNode>(name, makeConfig(), root);
  }

  NodeFactory factory_;
  std::shared_ptr<Blackboard> blackboard_;
  const XmlDocumentModel* model_ = nullptr;
  std::vector<std::string> subtree_stack_;
};
