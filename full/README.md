# full/ - MiniBT 完整集成版

这个目录是 MiniBT 的完整集成形态。按关卡学习请看 `../levels/`；想看一个可复用的功能模块版工程就看这里。

## 已包含能力

- `NodeStatus` / `ThreeNode` / `executeTick`
- `Sequence` / `Fallback`
- `StatefulAction` / `SleepAction`
- `DecoratorBase` / `RetryNode`
- `Blackboard`
- `PortsList` / `NodeConfig` / input-output remapping
- `NodeFactory` / `registerBuiltinNodes`
- TinyXML2 XML 解析
- XML AST 到运行时树实例化
- `SubTree` 与父子 Blackboard remapping
- `Tree` 薄封装

## 目录

```text
full/
  include/mini_bt/       完整头文件实现
  demos/main.cpp
  demos/enter_room.xml
```

## 构建

独立构建：

```bash
cmake -S full -B full/build
cmake --build full/build --config Release
```

从仓库根目录一起构建：

```bash
cmake -S . -B build -DMINI_BT_BUILD_FULL=ON
cmake --build build --config Release
```

## 示例

```bash
.\full\build\Release\mini_bt_full.exe
```

`mini_bt_full` 是最终闭环：

```text
XML 文件
  -> XmlParser
  -> FactoryXmlTreeBuilder
  -> NodeFactory
  -> NodeConfig ports remapping
  -> SubTree Blackboard remapping
  -> tickWhileRunning
```

成功时会检查：

```text
final_result = entered room via subtree
root status = SUCCESS
```
