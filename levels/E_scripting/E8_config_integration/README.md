# E8 - 配置文件生效

## 本关目标

打通 XML 属性进入脚本引擎并影响 tick 的完整链路。

```xml
<Sequence>
  <Script code="A:=0; ready:=false"/>
  <Work
      _skipIf="ready"
      _while="A &lt; 10"
      _onSuccess="ready:=true"/>
</Sequence>
```

## 完整链路

```text
XML 属性字符串
  -> XmlNode / NodeConfig 保存原文
  -> Factory 创建节点时 ParseScript
  -> ScriptFunction 存入节点 pre/post 数组
  -> executeTick 构造 Environment
  -> 脚本读写该树的 Blackboard
```

## 边界

要写：

- XML 保留属性识别
- `NodeConfig` 的 pre/post 原始脚本表
- Factory 建节点时编译脚本
- 非法脚本的建树期错误
- `Script code="..."` 属性接入

不要写：

- 在 XML Parser 中执行脚本
- 把脚本逻辑硬编码到业务节点
- 首次 tick 才解析固定属性

## 文件

| 文件 | 职责 |
|------|------|
| `script_node_config.hpp` | 原始 pre/post 脚本配置 |
| `script_xml_instantiation.hpp` | 属性分类与编译接入 |
| `e8_script_tree.xml` | 示例行为树 |
| `e8_config_integration.cpp` | 加载、建树、运行、检查 |

## 验收

- [ ] 只修改 XML 脚本即可改变行为。
- [ ] XML Parser 只保存字符串，不执行脚本。
- [ ] 非法脚本在建树期失败。
- [ ] Script 与业务节点使用同一个 Blackboard。
- [ ] XML 转义（如 `&lt;`）有测试。

通过后进入 [E9](../E9_hardening/)。
