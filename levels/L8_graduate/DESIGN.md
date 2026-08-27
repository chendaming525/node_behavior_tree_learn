# MiniBT ↔ BehaviorTree.CPP 设计对照（L8）

本文件是毕业关交付物：说明 MiniBT 学到的概念如何映射到官方 BehaviorTree.CPP。

## 1. 场景：进房间

`demos/l8_graduate.cpp` 实现：

```text
MainTree (Sequence)
├─ EnterRoomFallback (Fallback)
│    ├─ plan_A (Sequence)     ← 优先：电量够且门已开
│    │    BatteryOK → DoorOpen → EnterRoom
│    └─ plan_B (Sequence)     ← 备选：找钥匙、重试开门、等待、进入
│         FindKey → Retry(OpenDoor) → Sleep → EnterRoom → SetBlackboard
└─ SaySomething(result)       ← 从黑板读结果并打印
```

预期状态序列（口述用）：

1. Fallback 先 tick `plan_A`
2. `BatteryOK` SUCCESS → `DoorOpen` FAILURE → Sequence 调 `resetChildren()`，plan_A FAILURE
3. Fallback 换 `plan_B`：`FindKey` SUCCESS
4. `Retry` 下 `OpenDoor` 前两次 FAILURE，返回 RUNNING；第三次 SUCCESS
5. `Sleep(2)` 跨多次 tick 返回 RUNNING，然后 SUCCESS
6. `EnterRoom` + `SetBlackboard` SUCCESS → plan_B SUCCESS → Fallback SUCCESS
7. `SaySomething` 打印黑板 `result` → 整树 SUCCESS

## 2. 类映射表

| MiniBT | BehaviorTree.CPP | 说明 |
|--------|------------------|------|
| `ThreeNode` | `BT::TreeNode` | 统一 `tick` / `executeTick` / 状态 |
| `NodeStatus` | `BT::NodeStatus` | IDLE / RUNNING / SUCCESS / FAILURE |
| `ControlNode` + `resetChildren()` | `BT::ControlNode::resetChildren` | 结束或 halt 时清理孩子 |
| `SequenceNode` | `BT::SequenceNode` | 顺序 AND |
| `FallbackNode` | `BT::FallbackNode` | 备选 OR |
| `DecoratorBase` | `BT::DecoratorNode` | 单孩子 |
| `RetryNode` | `BT::RetryNode` / `RetryUntilSuccessful` | 失败重试 |
| `StatefulAction` | `BT::StatefulActionNode` | onStart / onRunning / onHalted |
| `SleepAction` | 自定义 Stateful 动作（如导航等待） | 协作式异步示例 |
| `Blackboard` | `BT::Blackboard` | 共享键值 |
| `getInput` / `setOutput` | Ports + remapping（简化版） | L5 用 key 直连黑板 |
| 手写 `addChild` 组树 | XML + `BehaviorTreeFactory` | 形态与实现分离的两种载体 |

## 3. 为何默认异步不是多线程？

行为树的 `RUNNING` 表示：**这次 tick 还没做完，下次主循环再来问**。

- 每次 `onStart` / `onRunning` 应很快返回，不占用控制周期。
- 真正的“等待时间”发生在两次 tick 之间（传感器、导航、墙钟时间），而不是在节点里 `sleep()` 阻塞线程。
- `Parallel` / 多个 RUNNING 也是同一线程里轮询各孩子，不是默认开线程。
- 官方另有 `ThreadedAction` 才是真线程；MiniBT 与官方推荐路径一致，优先 `StatefulAction`。

一句话：**异步 = 跨 tick 的状态机；并行语义 ≠ 多核并行。**

## 4. 设计哲学（回顾）

1. 编排（Control/Decorator）与行为（叶子）分离  
2. 统一状态契约，父节点只根据返回值调度  
3. 数据走黑板，不走 Control 传参  
4. 结束路径统一 `resetChildren()`，避免漏 halt  
5. 组合小节点表达复杂策略，而不是巨型 if/else  

## 5. 可选对照

官方入门：`BehaviorTree.CPP/examples/t01_build_your_first_tree.cpp`  
同样是 Sequence + 叶子；官方用 XML 描述形态，C++ 注册节点类型。

运行本 demo：

```bash
cmake -S . -B build
cmake --build build --config Release --target l8_graduate
./build/Release/l8_graduate.exe
```
