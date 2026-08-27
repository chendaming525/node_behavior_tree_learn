# L12 - XML 递归实例化

## 本关目标

把 L11 的 AST 变成真正能运行的行为树对象。

## 两个版本

本关保留两套构建器，方便对照学习：

- `XmlTreeBuilder`：手写 `if(type == "...")` 映射版本。
- `FactoryXmlTreeBuilder`：接入 `NodeFactory` 和 `providedPorts()` 的版本。

## 核心链路

```text
XmlNode.tag
  -> registration ID
  -> Factory builder
  -> ThreeNode
  -> 递归 build children
```

属性处理：

```text
XmlNode.attributes
  -> providedPorts manifest
  -> NodeConfig.input_ports / output_ports
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `xml_instantiation.hpp` | AST 到运行时树 |
| `bt_factory.hpp` | L12 本地 Factory |
| `register_builtin_nodes.hpp` | 注册普通节点 |
| `l12_enter_room.xml` | XML 版进房间 |
| `l12_xml_tree.cpp` | 分别运行 manual/factory 两版 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l12_xml_tree
```

## 验收

- manual 和 factory 两版都能跑到 `SUCCESS`。
- `Retry`、`Sleep` 能从 XML 属性读取参数。
- `SetBlackboard` / `SaySomething` 的 port 映射来自 XML。
- 未知标签、未知 port、错误 child 数会报错。
