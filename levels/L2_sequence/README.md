# L2 - Sequence 控制节点

## 本关目标

实现第一个控制节点 `SequenceNode`：按顺序执行孩子，遇到失败就失败，全部成功才成功。

## 边界

要写：

- `ControlNode`
- `addChild`
- `SequenceNode`

不要写：

- `Fallback`
- `Blackboard`
- `Retry`
- `halt` 传播

## 核心规则

```text
Sequence:
  child SUCCESS -> 继续下一个
  child FAILURE -> 整个 Sequence FAILURE
  child RUNNING -> 整个 Sequence RUNNING，并停在当前 child
  全部 SUCCESS -> 整个 Sequence SUCCESS
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `control_node.hpp` | 保存 children 和当前索引 |
| `sequence.hpp` | Sequence 调度规则 |
| `l2_sequence.cpp` | 组合几个叶子验证规则 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l2_sequence
```

## 验收

- 能解释为什么 Sequence 要记住当前 child index。
- 能说明 `RUNNING` 时为什么不能从头重跑。
- 能构造一个成功序列和一个失败序列。
