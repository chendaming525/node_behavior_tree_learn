# L4 - StatefulAction 与 RUNNING

## 本关目标

实现会跨多次 tick 才完成的动作节点，例如 `SleepAction`。

## 边界

要写：

- `StatefulAction`
- `onStart()` / `onRunning()` / `onHalted()`
- `SleepAction`

不要写：

- Retry
- Blackboard
- halt 传播到整棵树

## 核心理解

同步叶子一次 tick 就结束；状态型动作第一次进入时调用 `onStart()`，之后保持 `RUNNING` 时调用 `onRunning()`。

```text
IDLE -> onStart()
RUNNING -> onRunning()
halt -> onHalted() + reset
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `stateful_action.hpp` | 状态型动作模板 |
| `sleep_action.hpp` | 跨 tick 示例 |
| `l4_sleep.cpp` | 运行到 Sleep 完成 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l4_sleep
```

## 验收

- 能解释 `RUNNING` 节点为什么要保存内部进度。
- 能说清 `onStart` 和 `onRunning` 的触发条件。
- Sleep 能在多次 tick 后返回 `SUCCESS`。
