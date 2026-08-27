# L10 动手架子（本关目录，独立编译）

本关**只使用本目录文件**，不要引用 `full/`。

## 本关文件

| 文件 | 职责 |
|------|------|
| [`bt_factory.hpp`](bt_factory.hpp) | `NodeBuilder`、`NodeFactory`：注册表 + 按字符串实例化 |
| [`tree.hpp`](tree.hpp) | `Tree`：root + blackboard + tick 循环（可继续补全） |
| [`register_builtin_nodes.hpp`](register_builtin_nodes.hpp) | `registerBuiltinNodes()`：批量 `registerNodeType` |
| [`l10_factory.cpp`](l10_factory.cpp) | demo：用 Factory 组树并 tick |

依赖头文件（L9 及以前的本地副本）已放在本目录：`ports` / `node_config` / `three_node` / `sequence` / `enter_room_nodes` / `action_node` / `fallback` / `sleep_action` 等。

## 必须统一的 include 写法

```cpp
#include "bt_factory.hpp"
#include "enter_room_nodes.hpp"
```

L10 的 `CMakeLists.txt` 只把当前目录加入头文件搜索路径，不创建
`generated_include/mini_bt` 别名，也不搜索 `full/`。

## 构建

```bash
cmake -S levels -B build_levels -DMINI_BT_LEVEL_L10=ON
cmake --build build_levels --config Release --target level_l10_factory
.\build_levels\L10_factory\Release\level_l10_factory.exe
```

## 自检

- [ ] 用字符串 ID 注册并 `instantiateNode`
- [ ] Sequence 能挂上 Factory 创建的子节点并 tick
- [ ] 未注册 ID 会抛出带 ID 的错误
- [ ] （可选）`registerBuiltinNodes` + `Tree` 跑通 SetBlackboard → SaySomething
