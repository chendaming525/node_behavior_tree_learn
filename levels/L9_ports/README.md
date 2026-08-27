# L9 - Ports 与 NodeConfig

## 本关目标

把节点接口从“直接认 Blackboard key”升级为“声明 port，由实例配置 remapping”。

## 边界

要写：

- `InputPort` / `OutputPort`
- `PortsList`
- `NodeConfig.input_ports`
- `NodeConfig.output_ports`
- `getInput` / `setOutput`

不要写：

- Factory
- XML parser
- SubTree

## 核心理解

节点类型声明稳定接口：

```cpp
SaySomething::providedPorts() -> message input
```

节点实例决定接线：

```cpp
config.input_ports["message"] = "{result}";
```

因此同一个节点类型可以读不同 key，也可以读常量。

## 文件

| 文件 | 职责 |
| --- | --- |
| `ports.hpp` | port manifest 和 remapping 工具 |
| `node_config.hpp` | 每个实例的接线表 |
| `enter_room_nodes.hpp` | 使用 getInput/setOutput 的业务节点 |
| `l9_ports.cpp` | 常量输入和黑板输入 demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l9_ports
```

## 验收

- 能解释 port 名和 blackboard key 的区别。
- 输出 port 必须映射到 `{key}`。
- `SaySomething message="hello"` 和 `message="{result}"` 都能工作。
