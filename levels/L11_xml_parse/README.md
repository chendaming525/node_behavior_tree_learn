# L11 - XML 解析为 AST

## 本关目标

使用 TinyXML2 读取 XML，并转换成自己的 `XmlNode` AST。L11 只解析结构，不创建任何行为树节点。

## 边界

要写：

- `XmlNode`
- `XmlDocumentModel`
- `XmlParser::parseFile`
- `XmlParser::parseText`
- 主树 ID 校验

不要写：

- Factory
- `ThreeNode`
- `addChild`
- tick 运行

## 核心理解

```text
XML text/file
  -> tinyxml2 DOM
  -> XmlNode AST
```

`XmlNode` 保存：

```text
tag          标签名
attributes   XML 属性
children     有序子元素
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `xml_parser.hpp` | XML 到 AST |
| `enter_room.xml` | 解析样例 |
| `l11_parse_xml.cpp` | 打印 AST demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l11_parse_xml
```

## 验收

- 能从文件和字符串解析。
- 能保存 `main_tree_to_execute`。
- 能检测重复 `BehaviorTree ID`。
- `{result}` 这类 port 字符串原样保存。
