# L7 - halt 传播

## 本关目标

实现取消/复位语义：当一棵正在 `RUNNING` 的子树被打断时，`halt()` 要向下传播。

## 边界

要写：

- `ThreeNode::halt`
- Control 节点对子节点的 halt/reset
- StatefulAction 的 `onHalted`

不要写：

- XML
- Factory
- SubTree

## 核心理解

`RUNNING` 节点可能持有内部资源或进度。父节点不能只把状态改成 `IDLE`，还要通知正在运行的孩子清理自己。

## 文件

| 文件 | 职责 |
| --- | --- |
| `three_node.hpp` | 基础 halt/reset |
| `control_node.hpp` | 向孩子传播 halt |
| `sleep_action.hpp` | 可观察的 onHalted |
| `l7_halt.cpp` | 运行中打断 demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l7_halt
```

## 验收

- `RUNNING` 子节点被 halt 后能回到 `IDLE`。
- Sleep 的内部计数能被清理。
- 父节点 halt 时不会漏掉正在运行的孩子。
