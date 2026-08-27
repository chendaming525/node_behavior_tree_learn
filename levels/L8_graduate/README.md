# L8 - 手写树毕业关

## 本关目标

不用 XML，手写一棵完整的“进房间”行为树，把前面控制节点、装饰器、状态型动作、Blackboard 串起来。

## 场景

```text
Sequence MainTree
  Fallback EnterRoomFallback
    Sequence plan_A: BatteryOK -> DoorOpen -> EnterRoom
    Sequence plan_B: FindKey -> Retry(FlakyOpenDoor) -> Sleep -> EnterRoom -> SetBlackboard
  SaySomething
```

## 核心理解

L8 是手写版本的基线。后面的 XML、Factory、SubTree 都要能解释成和这棵树等价的运行行为。

## 文件

| 文件 | 职责 |
| --- | --- |
| `enter_room_nodes.hpp` | 业务叶子 |
| `retry_node.hpp` | 重试开门 |
| `sleep_action.hpp` | 等待动作 |
| `l8_graduate.cpp` | 手写组树并运行 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l8_graduate
```

## 验收

- plan_A 因门关闭失败。
- plan_B 通过重试、等待后成功。
- `SetBlackboard` 写入结果，`SaySomething` 读出结果。
- 整棵树最终返回 `SUCCESS`。
