#pragma once

#include <string>

namespace mini_bt
{

  enum class NodeStatus
  {
    IDLE = 0,
    RUNNING,
    SUCCESS,
    FAILURE
  };

  inline const char *toStr(NodeStatus status)
  {
    switch (status)
    {
    case NodeStatus::IDLE:
      return "IDLE";
    case NodeStatus::RUNNING:
      return "RUNNING";
    case NodeStatus::SUCCESS:
      return "SUCCESS";
    case NodeStatus::FAILURE:
      return "FAILURE";
    }
    return "UNKNOWN";
  }

} // namespace mini_bt
