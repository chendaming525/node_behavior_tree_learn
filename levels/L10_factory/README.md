# L10 - Factory 字符串创建节点

## 本关目标

建立从字符串 ID 到 C++ 节点类型的注册表，为 XML 标签创建运行时节点做准备。

## 边界

要写：

- `NodeFactory`
- `registerNodeType<T>(id)`
- `instantiateNode(id, name, config)`
- `registerBuiltinNodes`

不要写：

- XML parser
- 自动递归挂 children
- SubTree

## 核心理解

XML 里只有字符串：

```xml
<BatteryOK name="battery"/>
```

C++ 需要先注册：

```cpp
factory.registerNodeType<BatteryOK>("BatteryOK");
```

之后才能通过 `"BatteryOK"` 创建对象。

## 文件

| 文件 | 职责 |
| --- | --- |
| `bt_factory.hpp` | ID 到 builder 的映射 |
| `register_builtin_nodes.hpp` | 批量注册常用节点 |
| `l10_factory.cpp` | 注册并实例化 demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l10_factory
```

## 验收

- 重复注册会失败。
- 未注册 ID 会失败。
- Factory 能保存节点的 `providedPorts()`。
- 能用字符串创建普通节点实例。
