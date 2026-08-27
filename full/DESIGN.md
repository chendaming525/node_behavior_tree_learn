# MiniBT 完整版设计说明

`full/` 是完整集成工程，不按学习关卡组织。这里的代码按功能模块划分：

```text
include/mini_bt/
  types.hpp                 状态枚举
  three_node.hpp            节点基类
  control_node.hpp          多孩子控制节点基类
  sequence.hpp              Sequence 控制节点
  fallback.hpp              Fallback 控制节点
  decorator.hpp             单孩子装饰器基类
  retry_node.hpp            Retry 装饰器
  stateful_action.hpp       跨 tick 动作基类
  sleep_action.hpp          Sleep 示例动作
  blackboard.hpp            数据黑板与 SubTree remapping
  ports.hpp                 Port 声明和 remapping 工具
  node_config.hpp           节点实例接线配置
  bt_factory.hpp            字符串 ID 到节点构造器
  register_builtin_nodes.hpp 内置节点注册
  xml_parser.hpp            XML 到 AST
  xml_instantiation.hpp     AST 到运行时行为树
  tree.hpp                  根节点和 tick 封装
```

## 运行链路

```text
enter_room.xml
  -> XmlParser
  -> XmlDocumentModel / XmlNode
  -> FactoryXmlTreeBuilder
  -> NodeFactory
  -> ThreeNode runtime tree
  -> Blackboard / Port remapping
  -> tickWhileRunning
```

## 结构与行为分离

XML 只描述结构：

```xml
<SubTree ID="EnterRoomPlan" result="{final_result}"/>
<SaySomething message="{final_result}"/>
```

C++ 节点只描述行为：

```text
BatteryOK      检查电量
DoorOpen       检查门是否打开
FlakyOpenDoor  模拟开门失败后成功
SetBlackboard  写出结果
SaySomething   读取并打印结果
```

两者通过 `NodeFactory` 和 `NodeConfig` 连接。

## 数据流

Port 名是节点类型的稳定接口，Blackboard key 是实例之间的数据槽：

```text
SetBlackboard.text -> {result}
SubTree result     -> {final_result}
SaySomething       -> {final_result}
```

SubTree 会创建带 parent/remapping 的子 Blackboard，子树内部只认识自己的 key，父树只看边界映射后的 key。
