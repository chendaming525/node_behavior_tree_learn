# L1 - 节点基类与同步叶子

## 本关目标

引入行为树节点的共同外壳：所有节点都有名字、状态，并通过 `executeTick()` 被外部调用。

## 边界

要写：

- `ThreeNode`
- `tick()` / `executeTick()`
- `AlwaysSuccessNode`
- `AlwaysFailureNode`

不要写：

- 多孩子容器
- `Sequence` / `Fallback`
- `Blackboard`
- `RUNNING` 型长动作

## 核心理解

`tick()` 是业务子类实现的行为；`executeTick()` 是框架统一入口，负责调用 `tick()` 并保存返回状态。

```text
parent
  -> child->executeTick()
       -> child->tick()
       -> save status
       -> return status
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `three_node.hpp` | 节点基类 |
| `action_node.hpp` | 两个同步叶子 |
| `l1_leaves.cpp` | 调用并打印状态 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l1_leaves
```

## 验收

- 能说清 `tick()` 和 `executeTick()` 的区别。
- 能解释父节点为什么只调用 `executeTick()`。
- 两个同步叶子分别返回 `SUCCESS` 和 `FAILURE`。
