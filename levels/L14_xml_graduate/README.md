# L14 - XML 完整链路毕业关

## 本关目标

把 L0 到 L13 的能力串成一条完整闭环：从 XML 文件加载，到 Factory 实例化，到 SubTree remapping，到最终 tick 成功。

## 完整链路

```text
tinyxml2 读取 XML
  -> XmlParser 生成 AST
  -> FactoryXmlTreeBuilder 构建运行时树
  -> providedPorts 校验属性
  -> NodeConfig 保存 remapping
  -> Blackboard 读写数据
  -> SubTree 边界转发 key
  -> tickWhileRunning
```

## 场景

主树：

```text
Sequence
  SubTree EnterRoomPlan result="{final_result}"
  SaySomething message="{final_result}"
```

子树：

```text
Fallback
  plan_A: BatteryOK -> DoorOpen -> EnterRoom
  plan_B: FindKey -> Retry(FlakyOpenDoor) -> Sleep -> EnterRoom -> SetBlackboard
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `l14_enter_room.xml` | 最终 XML 场景 |
| `l14_xml_graduate.cpp` | 加载、构建、运行、检查结果 |
| `xml_instantiation.hpp` | 完整 XML 构建器 |
| `bt_factory.hpp` | Factory |
| `blackboard.hpp` | SubTree remapping 数据通道 |

## 运行

```bash
cmake --build build_levels --config Release --target level_l14_xml_graduate
```

## 验收

- 根节点最终返回 `SUCCESS`。
- Blackboard 中 `final_result` 等于 `entered room via subtree`。
- `SaySomething` 能打印 SubTree 写回父级的数据。
- 能解释结构、行为、接口、数据、调度分别由哪一层负责。

## 毕业后可选拓展

主线**不要求** Script / `_skipIf` / `_onSuccess`。若要学这套机制，走独立章节：

- [E_scripting](../E_scripting/)（拓展线 E0～E9，不是 L15）
