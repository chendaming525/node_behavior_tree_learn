# L0 - 环境与状态约定

## 本关目标

确认工程能独立编译运行，并定义行为树节点之间通用的返回状态。

本关只关心四个状态：

```text
IDLE      尚未运行或已复位
RUNNING   本次 tick 还没结束，下次继续
SUCCESS   本次任务成功结束
FAILURE   本次任务失败结束
```

## 边界

要写：

- `mini_bt::NodeStatus`
- `mini_bt::toStr(NodeStatus)`
- 一个最小 demo 打印状态

不要写：

- `ThreeNode`
- `Sequence` / `Fallback`
- `Blackboard`
- `halt`

## 核心理解

控制节点不应该理解业务返回值，例如 `open_ok`、`arrived`、`low_battery`。所有节点统一说 `RUNNING`、`SUCCESS`、`FAILURE`，父节点才能用通用规则调度孩子。

## 文件

| 文件 | 职责 |
| --- | --- |
| `types.hpp` | 定义状态枚举和字符串转换 |
| `l0_hello.cpp` | 最小运行 demo |

## 运行

```bash
cmake -S levels -B build_levels
cmake --build build_levels --config Release --target level_l0_hello
```

## 验收

- 能解释四个状态的含义。
- 知道业务节点正常不应该把 `IDLE` 当业务结果返回。
- 知道为什么 `RUNNING` 不能被 `bool` 替代。
