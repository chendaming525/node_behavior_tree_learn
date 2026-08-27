# E7 - 接入 executeTick

## 本关目标

把脚本闭包挂到节点执行前后，并实现专用脚本节点。

## 执行模型

```text
executeTick()
  -> checkPreConditions()
     -> 若短路：不调用业务 tick()
  -> tick()
  -> 若完成：checkPostConditions()
```

## 条件语义

Pre 顺序：

1. `_failureIf`
2. `_successIf`
3. `_skipIf`
4. `_while`

Post：

- `_onSuccess`
- `_onFailure`
- `_onHalted`
- `_post`

`_failureIf`、`_successIf`、`_skipIf` 仅在 IDLE/SKIPPED 时检查；`_while` 在 RUNNING 时也要复查，变假则 halt。

## 边界

要写：

- pre/post 的 `ScriptFunction` 容器
- `checkPreConditions` / `checkPostConditions`
- `ScriptNode`
- `ScriptCondition`

不要写：

- XML 属性加载
- 每 tick 重新解析固定脚本

## 文件

| 文件 | 职责 |
|------|------|
| `script_conditions.hpp` | pre/post 枚举与容器 |
| `script_node.hpp` | 执行副作用并返回 SUCCESS |
| `script_condition.hpp` | 条件结果映射 SUCCESS/FAILURE |
| `e7_tick_hooks.cpp` | 时序与短路测试 |

## 验收

- [ ] `_skipIf="done"` 为真时业务 `tick()` 未被调用。
- [ ] `_onSuccess="n:=n+1"` 在成功后修改 BB。
- [ ] RUNNING 时只有 `_while` 被持续检查。
- [ ] `_while` 变假会 halt 正在运行的节点。
- [ ] pre/post 执行顺序有测试锁定。

通过后进入 [E8](../E8_config_integration/)。
