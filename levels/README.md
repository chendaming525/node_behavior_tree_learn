# levels/ 关卡阅读指南

`levels/` 里每个目录是一关，**只使用本目录文件构建**，不再引用 `full/`。

缺头文件或 demo 时，应在**本关目录里创建**，而不是去链接完整工程。

## 构建（可勾选每一关）

```bash
cmake -S levels -B build_levels
# cmake-gui 里勾选 MINI_BT_LEVEL_L0 … L14 以及 MINI_BT_LEVEL_E0 … E9
cmake --build build_levels --config Release --target level_l9_ports
.\build_levels\L9_ports\Release\level_l9_ports.exe
```

| 选项 | 含义 |
|------|------|
| `MINI_BT_LEVEL_L0` … `L14` | 是否构建对应主线关卡 |
| `MINI_BT_LEVEL_E_SCRIPTING` | 拓展线总开关（默认 OFF）；勾选后 E0～E9 才出现在列表里，关闭则隐藏且不编 |
| `MINI_BT_LEVEL_E0` … `E9` | 依赖总开关；尚无本关 `CMakeLists.txt` 时会跳过 |

也可从仓库根构建（默认已开 `MINI_BT_BUILD_LEVELS=ON`；`MINI_BT_BUILD_FULL` 默认 OFF）。

每个关卡自己的 `CMakeLists.txt` 会把本目录全部 `.hpp` 映射成 `#include "mini_bt/xxx.hpp"`。

## 关卡列表

| 关卡 | 目录 | 本关只解决的问题 | 可执行目标 |
|------|------|------------------|------------|
| L0 | [L0_environment](L0_environment/) | 统一四种节点状态 | `level_l0_hello` |
| L1 | [L1_sync_leaves](L1_sync_leaves/) | 节点基类和同步叶子 | `level_l1_leaves` |
| L2 | [L2_sequence](L2_sequence/) | Sequence 顺序流程 | `level_l2_sequence` |
| L3 | [L3_fallback](L3_fallback/) | Fallback 备选方案 | `level_l3_door` |
| L4 | [L4_stateful_async](L4_stateful_async/) | RUNNING 跨 tick | `level_l4_sleep` |
| L5 | [L5_blackboard](L5_blackboard/) | 黑板传数据 | `level_l5_blackboard` |
| L6 | [L6_retry_decorator](L6_retry_decorator/) | Retry 装饰器 | `level_l6_retry` |
| L7 | [L7_halt_propagation](L7_halt_propagation/) | halt 传播 | `level_l7_halt` |
| L8 | [L8_graduate](L8_graduate/) | 手写总装进房间 | `level_l8_graduate` |
| L9 | [L9_ports](L9_ports/) | Port remapping | `level_l9_ports` |
| L10 | [L10_factory](L10_factory/) | Factory 字符串实例化 | `level_l10_factory`（默认 OFF，自己实现） |
| L11 | [L11_xml_parse](L11_xml_parse/) | XML 解析 | `level_l11_parse_xml`（占位） |
| L12 | [L12_xml_tree](L12_xml_tree/) | XML 建树 | `level_l12_xml_tree`（占位） |
| L13 | [L13_subtree](L13_subtree/) | SubTree remapping | `level_l13_subtree`（占位） |
| L14 | [L14_xml_graduate](L14_xml_graduate/) | XML 端到端 | `level_l14_xml_graduate`（占位） |

### 拓展线（独立章节，默认不进上方勾选）

| 关卡 | 目录 | 本关只解决的问题 | 可执行目标 |
|------|------|------------------|------------|
| E0 | [E0_language](E_scripting/E0_language/) | 定义语言边界 | 无可执行文件（文档关） |
| E1 | [E1_environment](E_scripting/E1_environment/) | ScriptValue / Blackboard / Environment | `level_e1_environment` |
| E2 | [E2_tokenizer](E_scripting/E2_tokenizer/) | 词法分析 | `level_e2_tokenizer`（默认 OFF） |
| E3 | [E3_ast](E_scripting/E3_ast/) | AST 与求值语义 | 默认 OFF |
| E4 | [E4_pratt_parser](E_scripting/E4_pratt_parser/) | Pratt 表达式解析 | 默认 OFF |
| E5 | [E5_assignment](E_scripting/E5_assignment/) | 赋值与多语句 | 默认 OFF |
| E6 | [E6_script_function](E_scripting/E6_script_function/) | 编译成可复用闭包 | 默认 OFF |
| E7 | [E7_tick_hooks](E_scripting/E7_tick_hooks/) | pre/post 与 Script 节点 | 默认 OFF |
| E8 | [E8_config_integration](E_scripting/E8_config_integration/) | XML 属性生效 | 默认 OFF |
| E9 | [E9_hardening](E_scripting/E9_hardening/) | 枚举、缓存、错误和测试 | 默认 OFF |

拓展线总览见 [E_scripting](E_scripting/)，完整正文见 [QUEST 拓展线](../QUEST.md#E-scripting)。它不是 L15。

完整集成工程仍在 [`../full/`](../full/)，与关卡构建互不依赖。
