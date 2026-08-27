# L5 - Blackboard 数据共享

## 本关目标

引入 `Blackboard`，让一个节点写出的数据能被后面的节点读取。

## 边界

要写：

- `Blackboard::set/get`
- 写消息节点
- 读消息节点

不要写：

- Port manifest
- XML 属性映射
- SubTree 局部黑板

## 核心理解

行为树结构负责调度，Blackboard 负责数据流。

```text
SetBlackboard -> BB["msg"] = "hello"
SaySomething  -> read BB["msg"]
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `blackboard.hpp` | key/value 数据存储 |
| `three_node_with_blackboard.hpp` | 带黑板的节点版本 |
| `l5_blackboard.cpp` | 写入再读取 demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l5_blackboard
```

## 验收

- 能解释 Blackboard 和节点状态是两件事。
- 能让前一个节点写入，后一个节点读取。
- 缺少 key 时有明确错误或失败路径。
