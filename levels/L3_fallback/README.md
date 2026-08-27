# L3 - Fallback 控制节点

## 本关目标

实现 `FallbackNode`，用于“先试方案 A，失败后再试方案 B”的选择逻辑。

## 边界

要写：

- `FallbackNode`
- 一个进房间小场景：门关了就找钥匙

不要写：

- Retry
- Blackboard
- XML
- halt 传播

## 核心规则

```text
Fallback:
  child FAILURE -> 继续下一个
  child SUCCESS -> 整个 Fallback SUCCESS
  child RUNNING -> 整个 Fallback RUNNING，并停在当前 child
  全部 FAILURE -> 整个 Fallback FAILURE
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `fallback.hpp` | Fallback 调度规则 |
| `l3_door.cpp` | plan_A / plan_B 场景 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l3_door
```

## 验收

- 能对比 Sequence 和 Fallback 的成功/失败推进方向。
- 能解释 plan_A 失败后为什么会进入 plan_B。
- 能画出 `Fallback(Sequence(...), Sequence(...))` 的结构。
