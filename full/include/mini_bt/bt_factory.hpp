#pragma once

#include "mini_bt/node_config.hpp"
#include "mini_bt/ports.hpp"
#include "mini_bt/three_node.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

using NodeBuilder = std::function<std::shared_ptr<ThreeNode>(
    const std::string& name,
    const NodeConfig& config)>;

template <typename T, typename = void>
struct HasProvidedPorts : std::false_type
{
};

template <typename T>
struct HasProvidedPorts<T, std::void_t<decltype(T::providedPorts())>> : std::true_type
{
};

template <typename T>
PortsList getProvidedPortsOf()
{
  if constexpr(HasProvidedPorts<T>::value)
  {
    return T::providedPorts();
  }
  return {};
}

class NodeFactory
{
public:
  void registerBuilder(
      const std::string& registration_id,
      NodeBuilder builder,
      PortsList ports_manifest = {})
  {
    if(builders_.count(registration_id) != 0)
    {
      throw std::runtime_error("duplicate node registration: " + registration_id);
    }

    builders_[registration_id] = std::move(builder);
    manifests_[registration_id] = std::move(ports_manifest);
  }

  template <typename T>
  void registerNodeType(const std::string& registration_id)
  {
    registerBuilder(
        registration_id,
        [](const std::string& name, const NodeConfig& config) {
          return std::make_shared<T>(name, config);
        },
        getProvidedPortsOf<T>());
  }

  std::shared_ptr<ThreeNode> instantiateNode(
      const std::string& registration_id,
      const std::string& name,
      const NodeConfig& config) const
  {
    const auto it = builders_.find(registration_id);
    if(it == builders_.end())
    {
      throw std::runtime_error("unknown node registration: " + registration_id);
    }
    return it->second(name, config);
  }

  bool isRegistered(const std::string& registration_id) const
  {
    return builders_.count(registration_id) != 0;
  }

  PortsList getPortsList(const std::string& registration_id) const
  {
    const auto it = manifests_.find(registration_id);
    if(it == manifests_.end())
    {
      throw std::runtime_error("unknown node registration: " + registration_id);
    }
    return it->second;
  }

private:
  std::unordered_map<std::string, NodeBuilder> builders_;
  std::unordered_map<std::string, PortsList> manifests_;
};
