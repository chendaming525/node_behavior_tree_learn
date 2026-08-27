# 拓展线 E · 嵌入式脚本（独立章节）

> **不是 L15。** 不属于主线 L0～L14 续关，也不计入主线毕业条件。  
> 完整关卡正文在仓库手册：[QUEST.md → 拓展线 E0～E9](../../QUEST.md#E-scripting)。

## 你在学什么

做一套与 BehaviorTree.CPP **同构** 的微型脚本机制：

```text
配置里的短字符串
  -> tokenize
  -> Pratt 解析成 AST
  -> 编译成 ScriptFunction（闭包）
  -> 在 executeTick 的 pre/post（或 Script 节点）里执行
  -> 只读写 Blackboard
```

典型配置形态：

```xml
<Script code="A:=1; msg:='hi'"/>
<Work _skipIf="ready" _onSuccess="n:=n+1"/>
```

## 和主线 / 支线的关系

| 路线 | 编号 | 关系 |
|------|------|------|
| 主线 | L0～L14 | 本线**不接在后面**；L14 刻意不做脚本 |
| 支线 | B1～B5 | 其它可选能力；与本线并列 |
| **拓展线** | **E0～E9** | 本目录；主线通关后按兴趣选修 |

**建议前置：** 至少理解 L5 Blackboard；做 E7/E8 时最好已做过 `executeTick` 与 XML 建树（L12/L14）。

## 分关目录

每一关都是独立目录，和主线 `L0_*`～`L14_*` 的组织方式一致。先读该关 README，再只在该关目录内实现。

| 关 | 独立目录 | 目标 | QUEST |
|----|----------|------|-------|
| E0 | [E0_language/](E0_language/) | 定语言边界（别做成 Lua） | [正文](../../QUEST.md#E0) |
| E1 | [E1_environment/](E1_environment/) | `Environment` + Any/Blackboard | [正文](../../QUEST.md#E1) |
| E2 | [E2_tokenizer/](E2_tokenizer/) | Tokenizer | [正文](../../QUEST.md#E2) |
| E3 | [E3_ast/](E3_ast/) | AST + 手写求值 | [正文](../../QUEST.md#E3) |
| E4 | [E4_pratt_parser/](E4_pratt_parser/) | Pratt 解析器（核心） | [正文](../../QUEST.md#E4) |
| E5 | [E5_assignment/](E5_assignment/) | 赋值 `:=` / 多语句 `;` | [正文](../../QUEST.md#E5) |
| E6 | [E6_script_function/](E6_script_function/) | `ParseScript` → 闭包 | [正文](../../QUEST.md#E6) |
| E7 | [E7_tick_hooks/](E7_tick_hooks/) | 挂到 `executeTick` pre/post | [正文](../../QUEST.md#E7) |
| E8 | [E8_config_integration/](E8_config_integration/) | XML 属性生效整条链路 | [正文](../../QUEST.md#E8) |
| E9 | [E9_hardening/](E9_hardening/) | 枚举、错误、缓存、测试 | [正文](../../QUEST.md#E9) |

## 怎么练

1. 只在当前 `E*_*/` 关卡目录里新增代码与测试，**不要改**已过关的 E 关或 `L0`～`L14` 目录。  
2. 按 E0 → E9 顺序；一次只做一关。  
3. 用 [QUEST.md](../../QUEST.md) 里每关的「自检」打勾。  
4. 对照只读：同级仓库 `BehaviorTree.CPP` 的 `scripting/`、`tree_node.cpp`、`examples/t09_scripting.cpp`。

构建开关在 [`levels/CMakeLists.txt`](../CMakeLists.txt) 里，和 L0～L14 同一页：

| 选项 | 默认 | 作用 |
|------|------|------|
| `MINI_BT_LEVEL_E_SCRIPTING` | OFF | 总开关；勾选后 E0～E9 才显示 |
| `MINI_BT_LEVEL_E0` | OFF | 依赖总开关；文档关，无目标 |
| `MINI_BT_LEVEL_E1` | ON | 依赖总开关；`level_e1_environment` |
| `MINI_BT_LEVEL_E2` … `E9` | OFF | 依赖总开关；有本关 `CMakeLists.txt` 才编译 |

在 cmake-gui 中：先勾选 `MINI_BT_LEVEL_E_SCRIPTING` → Configure，E0～E9 才会出现；取消总开关再 Configure，它们会隐藏并强制为 OFF。

```bash
cmake -S levels -B build_levels
cmake --build build_levels --config Release --target level_e1_environment
```

根工程只需打开 `MINI_BT_BUILD_LEVELS`（默认 ON）。若日后要并入 `full/`，单独开集成任务，仍不要改主线关号。

## 最小里程碑（E6 过后应能跑）

```cpp
auto set  = ParseScript("A:=1; msg:='hi'").value();
auto cond = ParseScript("A > 0 && msg == 'hi'").value();
set(env);
// cond(env) 为真
```

## 验收（整线）

- 能解释：为何先编译成闭包，而不是每 tick 解析字符串。  
- 能画出：XML 属性 → `NodeConfig` 字符串 → `ParseScript` → pre/post 执行 → Blackboard。  
- 能说明：本线与主线 L14 的边界（L14 不要求脚本）。
