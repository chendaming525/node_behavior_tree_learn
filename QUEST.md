# MiniBT 闯关手册（可脱离 Cursor 独立使用）

本手册是**自包含**学习路线。你不需要 Cursor Canvas，也不需要 AI 在线。

- **按关动手**：[`levels/`](levels/)（每关一个独立工程，只改本关目录）
- **完整集成对照**（可选，默认不编）：[`full/`](full/)，与关卡互不引用
- 玩法：按关卡改本关文件 → 用 `levels/` 编译运行 → 对照「自检清单」打勾 → 进入下一关
- 原则：一次只做一关；不要提前实现下一关；不要 `#include` 或链接 `full/`

---

## 快速开始（现在就能跑）

```bash
cmake -S levels -B build_levels
cmake --build build_levels --config Release --target level_l8_graduate
# Windows / MSVC 常见路径：
.\build_levels\L8_graduate\Release\level_l8_graduate.exe
```

关卡工程已做到 **L9 Ports**；L10 可独立编译。L11～L14 按本文在对应 `levels/Lx_*` 目录继续实现。`full/` 是另一套独立工程，需要时：`cmake -S full -B full/build`。
---

## 口令（可选：有 AI 时）

| 你说 | 作用 |
|------|------|
| 开始 Lx | 让 AI 只发本关提示 |
| 验收 Lx | 让 AI 帮你判关 |
| 提示 Lx | 只要提示不要完整答案 |

**没有 AI 时**：只用本文「自检清单」即可闯关。

---

## 关卡列表清单

用本表核对进度，并跳转到对应章节。勾选框可自行打勾。

### 主线：运行时核心（L0～L8）

| 进度 | 关卡 | 一句话目标 | 核心交付 | 跳转 |
|------|------|------------|----------|------|
| [ ] | L0 | 统一四种节点状态 | `types.hpp` + `l0_hello` | [→ L0](#L0) |
| [ ] | L1 | 节点基类 + 同步叶子 | `three_node.hpp` + AlwaysSuccess/Failure | [→ L1](#L1) |
| [ ] | L2 | Sequence 顺序编排 | `control_node.hpp` + `sequence.hpp` | [→ L2](#L2) |
| [ ] | L3 | Fallback 备选方案 | `fallback.hpp` + 进房间树雏形 | [→ L3](#L3) |
| [ ] | L4 | 协作式异步 RUNNING | `stateful_action.hpp` + `SleepAction` | [→ L4](#L4) |
| [ ] | L5 | 节点间传数据 | `blackboard.hpp` + getInput/setOutput | [→ L5](#L5) |
| [ ] | L6 | Retry 装饰器 | `decorator.hpp` + `retry_node.hpp` | [→ L6](#L6) |
| [ ] | L7 | halt / reset 传播 | halt + resetChildren | [→ L7](#L7) |
| [ ] | L8 | 手写树总装（中期里程碑） | `l8_graduate` + `DESIGN.md` | [→ L8](#L8) |

### 主线：描述与实现分离（L9～L14）

| 进度 | 关卡 | 一句话目标 | 核心交付 | 跳转 |
|------|------|------------|----------|------|
| [ ] | L9 | Port 接口与黑板映射 | `ports.hpp` + `node_config.hpp` | [→ L9](#L9) |
| [ ] | L10 | 按字符串注册/实例化节点 | `bt_factory.hpp` + `tree.hpp` | [→ L10](#L10) |
| [ ] | L11 | XML 只解析成 AST | `xml_parser` + AST，不建树 | [→ L11](#L11) |
| [ ] | L12 | XML 递归实例化整棵树 | `createTreeFromText/File` | [→ L12](#L12) |
| [ ] | L13 | SubTree 跨黑板 remapping | `subtree.hpp` + 父子 BB | [→ L13](#L13) |
| [ ] | L14 | 完整链路毕业 | XML 进房间 + 最小测试 | [→ L14](#L14) |

### 支线（主线全通后再做）

| 进度 | 关卡 | 一句话目标 | 跳转 |
|------|------|------------|------|
| [ ] | B1 | 极简缩进文本组树（XML 前热身） | [→ B1](#B1) |
| [ ] | B2 | Timeout（按 tick 计数） | [→ B2](#B2) |
| [ ] | B3 | ReactiveSequence | [→ B3](#B3) |
| [ ] | B4 | 插件动态加载 | [→ B4](#B4) |
| [ ] | B5 | Groot2 可视化与监控 | [→ B5](#B5) |

### 拓展线：嵌入式脚本（独立章节，不是 L15）

> **不属于主线续关。** 不接在 L14 后面编号；主线毕业条件不变。  
> 建议主线至少完成 **L5（Blackboard）**；若要做「XML 属性生效」，再具备 **L12/L14** 经验。  
> 总目录：[levels/E_scripting/](levels/E_scripting/) · 总述：[→ 拓展线](#E-scripting)

| 进度 | 关卡 | 一句话目标 | 核心交付 | 跳转 |
|------|------|------------|----------|------|
| [ ] | E0 | 定语言边界（别做成 Lua） | 一页语法说明书 | [→ E0](#E0) |
| [ ] | E1 | 脚本运行环境 | `Environment` + Any | [→ E1](#E1) |
| [ ] | E2 | 词法分析 | `tokenize` | [→ E2](#E2) |
| [ ] | E3 | AST + 手写求值 | `Expr*` | [→ E3](#E3) |
| [ ] | E4 | Pratt 解析器 | 表达式 → AST | [→ E4](#E4) |
| [ ] | E5 | 赋值与多语句 | `:=` / `;` | [→ E5](#E5) |
| [ ] | E6 | 编译成闭包 | `ParseScript` | [→ E6](#E6) |
| [ ] | E7 | 挂到宿主 tick | pre/post + Script 节点 | [→ E7](#E7) |
| [ ] | E8 | 配置文件生效 | XML 属性 → 编译 → 执行 | [→ E8](#E8) |
| [ ] | E9 | 加固到能用 | 枚举、错误、缓存、测试 | [→ E9](#E9) |

### 整条链路一览

```text
L0 状态契约
  -> L1 节点基类
  -> L2 Sequence / L3 Fallback
  -> L4 RUNNING 异步
  -> L5 Blackboard（简化 key）
  -> L6 Decorator/Retry
  -> L7 halt 传播
  -> L8 手写组树（中期毕业）
  -> L9 Ports（形参/返回值 + remapping）
  -> L10 Factory（字符串 ID -> C++ 对象）
  -> L11 XML AST
  -> L12 XML 建树并 tick
  -> L13 SubTree remapping
  -> L14 端到端闭环

拓展线（选修，非续关）:
  E0 语言边界 -> E1 Env -> E2 Token -> E3 AST
    -> E4 Pratt -> E5 赋值 -> E6 ParseScript
    -> E7 tick 钩子 -> E8 配置生效 -> E9 加固
```

**L8 ≠ 终点。** L8 证明“运行时能跑”；L14 证明“结构、行为、接口、数据、调度”分层闭环。  
**E 线 ≠ L15。** 脚本是拓展能力，不改变主线毕业定义。

---

<a id="L0"></a>
## L0 · 环境与契约

**章节副本**：[levels/L0_environment/](levels/L0_environment/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

- 确认工程能编译运行。
- 定义节点之间唯一通用的返回状态：`IDLE` / `RUNNING` / `SUCCESS` / `FAILURE`。

学完后应能解释：父节点为什么不关心“开门”“找钥匙”，只关心孩子这次 tick 返回了什么状态。

### 需求边界

**要写：**

- `enum class NodeStatus`
- `toStr(NodeStatus)`
- 最小 demo：打印四种状态

**不要写：**

- `ThreeNode`、`executeTick`、Sequence/Fallback、Sleep、Blackboard、halt、XML

### 推荐设计

```text
IDLE      未启动，或已被复位
RUNNING   本次未完成，下次 tick 继续
SUCCESS   成功结束
FAILURE   失败结束
```

### 设计意图和意义

行为树靠父节点调度孩子。若每个节点返回 `open_ok` / `arrived` / `try_again` 之类私有语言，控制节点必然耦合所有业务。

统一四种状态后：

| 状态 | 父节点含义 |
|------|------------|
| RUNNING | 孩子还在忙，停在这里 |
| SUCCESS | 可按规则推进 |
| FAILURE | 可按规则失败或换方案 |
| IDLE | 框架复位态，一般不当业务结果返回 |

`bool` 不够：表达不了“还在进行中”。`RUNNING` 是异步、重试、取消的基础。

### 交付文件

```text
levels/L0_environment/types.hpp
levels/L0_environment/l0_hello.cpp
# 目标 level_l0_hello
```

### 实现顺序

1. 写 `NodeStatus`。
2. 写 `toStr`。
3. demo 打印 `Hello MiniBT` 与四状态。
4. 编译运行。

### 对照文件

| 本关 | 最终工程 |
|------|----------|
| `types.hpp` | `levels/L0_environment/types.hpp` |
| `l0_hello.cpp` | `levels/L0_environment/l0_hello.cpp` |

### 自检

- [ ] 构建无错误
- [ ] 终端出现 `Hello MiniBT`
- [ ] 打印了 IDLE / RUNNING / SUCCESS / FAILURE
- [ ] 能说清四状态含义；知道业务 tick 一般不返回 IDLE

### 通过后

进入 [L1](#L1)。勾上清单 L0。

---

<a id="L1"></a>
## L1 · 同步叶子与节点基类

**章节副本**：[levels/L1_sync_leaves/](levels/L1_sync_leaves/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

建立统一调用接口：

- 节点有名字与当前状态
- 外部只调 `executeTick()`
- 子类只实现 `tick()`
- 两个同步叶子：`AlwaysSuccess` / `AlwaysFailure`

学完后应知道：控制节点为何能把任何孩子都当成同一基类调用。

### 需求边界

**要写：**

- `ThreeNode`（或你命名的 `TreeNode`）
- `executeTick()` / `curStatus()` / `setStatus()`
- `AlwaysSuccessNode` / `AlwaysFailureNode`

**不要写：**

- 多孩子容器、Sequence/Fallback、Blackboard、Retry、halt 传播、StatefulAction

### 推荐设计

```text
executeTick():
  status = tick()      # 子类业务
  setStatus(status)    # 基类记账
  return status
```

同步叶子只返回终态，**禁止**返回 `RUNNING`（可 assert/throw）。

### 设计意图和意义

| 函数 | 谁写 | 职责 |
|------|------|------|
| `tick()` | 业务子类 | 做一次推进，返回状态 |
| `executeTick()` | 基类 | 统一调用 + 保存状态 |

父节点只写 `child->executeTick()`，无需知道孩子是开门还是子树。这是组合的前提。

### 交付文件

```text
levels/L1_sync_leaves/three_node.hpp
levels/L1_sync_leaves/action_node.hpp
levels/L1_sync_leaves/l1_leaves.cpp
# 目标 level_l1_leaves
```

### 实现顺序

1. 写基类外壳（name + status）。
2. 纯虚 `tick()` + `executeTick()`。
3. 写 AlwaysSuccess / AlwaysFailure。
4. demo：各 `executeTick()` 一次并打印 `[name] -> status`。

### 对照文件

| 本关 | 最终工程 |
|------|----------|
| `three_node.hpp` | `levels/L1_sync_leaves/three_node.hpp` |
| `action_node.hpp` | `levels/L1_sync_leaves/action_node.hpp` |

> 最终工程的 `ThreeNode` 在 L5/L7 后会多出黑板与 halt；读 L1 时先忽略后续能力。

### 自检

- [ ] AlwaysSuccess → SUCCESS；AlwaysFailure → FAILURE
- [ ] 能默写 `executeTick()` 三步
- [ ] 同步叶子返回 RUNNING 时有明确错误（推荐）

### 通过后

进入 [L2](#L2)。

---

<a id="L2"></a>
## L2 · Sequence

**章节副本**：[levels/L2_sequence/](levels/L2_sequence/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

实现第一个控制节点 `SequenceNode`：

```text
从左到右 tick 孩子
全部 SUCCESS -> Sequence SUCCESS
任一 FAILURE -> Sequence FAILURE
任一 RUNNING -> Sequence RUNNING，下次从该孩子继续
```

### 需求边界

**要写：**

- `ControlNode`：多孩子 + `addChild`
- `SequenceNode` + `current_child_index_` 书签
- Demo A：三 AlwaysSuccess → SUCCESS
- Demo B：Success, Failure, Success → 第三人不得被 tick

**不要写：**

- Fallback、Blackboard 传播、Decorator、完整 `resetChildren()` / halt（留给 L7）

本关成功/失败后清零书签即可；深度 halt 留到 L7。

### 推荐设计

```text
while index < children.size():
  status = children[index]->executeTick()
  if RUNNING: return RUNNING
  if FAILURE: index = 0; return FAILURE
  index += 1
index = 0
return SUCCESS
```

### 设计意图和意义

Sequence 把“必须按顺序都做成”从业务 `if` 里抽到树结构。书签让 `RUNNING` 与顺序流程共存：中间孩子 RUNNING 时，下次不应从第一个孩子重来。

### 交付文件

```text
levels/L2_sequence/control_node.hpp
levels/L2_sequence/sequence.hpp
levels/L2_sequence/l2_sequence.cpp
```

### 实现顺序

1. `ControlNode` 只管孩子列表。
2. 写 Sequence while 循环。
3. Demo A / Demo B。
4. （可选）假异步叶子验证书签。

### 对照文件

| 本关 | 最终工程 |
|------|----------|
| `control_node.hpp` | `levels/L2_sequence/control_node.hpp` |
| `sequence.hpp` | `levels/L2_sequence/sequence.hpp` |

### 自检

- [ ] Demo A 最终 SUCCESS
- [ ] Demo B 日志看不到第三个孩子
- [ ] 能解释 RUNNING 时为何记住下标

### 通过后

进入 [L3](#L3)。

---

<a id="L3"></a>
## L3 · Fallback + 进房间雏形

**章节副本**：[levels/L3_fallback/](levels/L3_fallback/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

实现 `FallbackNode`，用树表达“先方案 A，不行再方案 B”。

```text
任一 SUCCESS -> Fallback SUCCESS
任一 RUNNING -> RUNNING（记住方案）
全部 FAILURE -> FAILURE
```

并组出学习场景树：

```text
Fallback
  Sequence(BatteryOK, DoorOpen, EnterRoom)   # 方案 A
  Sequence(FindKey, OpenDoor, EnterRoom)     # 方案 B
```

### 需求边界

**要写：** Fallback + 简单 Condition/Action 叶子（BatteryOK、DoorOpen 可固定 FAILURE 以逼出方案 B）。

**不要写：** Retry、Blackboard、完整 halt。

### 推荐设计

Fallback 与 Sequence 对称：成功则整段成功并清零；失败则换下一个孩子。

### 设计意图和意义

没有 Fallback 时，备选逻辑会塞进业务节点（开门内部又找钥匙又报警）。Fallback 把策略选择放回树：每个孩子是一种方案；方案内部用 Sequence 表达步骤。

### 交付文件

```text
levels/L3_fallback/fallback.hpp
levels/L3_fallback/l3_door.cpp
```

### 实现顺序

1. 复用 `ControlNode` 写 Fallback。
2. 单测 FAILURE, FAILURE, SUCCESS。
3. 组合两个 Sequence 口述方案 A/B。
4. 跑进房间 demo。

### 自检

- [ ] 日志先走方案 A 失败，再走方案 B
- [ ] 根节点最终 SUCCESS
- [ ] 能说清 Sequence vs Fallback

### 通过后

进入 [L4](#L4)。

---

<a id="L4"></a>
## L4 · StatefulAction 协作式异步

**章节副本**：[levels/L4_stateful_async/](levels/L4_stateful_async/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

长时间动作不能一次 tick 完成：返回 `RUNNING`，下次再继续。

实现：

- `StatefulAction`：`onStart` / `onRunning` / `onHalted`
- `SleepAction(ticks_needed)`：**用计数器**，禁止 `sleep_for`
- `tickWhileRunning(root)` 循环直到非 RUNNING

### 需求边界

**要写：** Stateful 分发 + Sleep + 循环 demo。

**不要写：** Retry、Blackboard、完整 halt 传播、真线程。

### 推荐设计

```text
IDLE     -> onStart()
RUNNING  -> onRunning()
```

Sleep：第 1 次 onStart 清零；后续 onRunning 计数，未到阈值返回 RUNNING。

### 设计意图和意义

导航/等待若在 `tick()` 里阻塞，整棵树卡住。协作式异步 = **跨 tick 的状态机**，不是默认开线程。官方另有 `ThreadedAction`；MiniBT 与推荐路径一致，优先 Stateful。

### 交付文件

```text
levels/L4_stateful_async/stateful_action.hpp
levels/L4_stateful_async/sleep_action.hpp
levels/L4_stateful_async/l4_sleep.cpp
```

### 实现顺序

1. Stateful 抽象与分发。
2. Sleep 计数。
3. Sequence(AlwaysSuccess, Sleep(3), AlwaysSuccess)。
4. 循环 tick 到 SUCCESS。

### 自检

- [ ] 至少 3 次根节点 RUNNING，最后 SUCCESS
- [ ] 节点内无 `sleep_for`
- [ ] 能解释异步 ≠ 多线程

### 通过后

进入 [L5](#L5)。

---

<a id="L5"></a>
## L5 · Blackboard

**章节副本**：[levels/L5_blackboard/](levels/L5_blackboard/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

解决数据流：一个节点写出的值，另一个节点怎么拿到。

实现：

- `Blackboard`：`set` / `get<T>`（可用 `std::any`）
- 节点持有 `shared_ptr<Blackboard>`
- `setOutput` / `getInput`（本关可直接用黑板 key）
- `SetBlackboard` 写、`SaySomething` 读
- `Sequence(Set, Say)`

### 需求边界

**要写：** 共享键值表 + 黑板传播到孩子。

**不要写：** 完整 Port remapping 语法、XML、Retry、halt 细节。

> L5 是“能传数据”；正式 Port / 线的概念在 [L9](#L9)。

### 推荐设计

```text
demo 创建 bb
root->setBlackboard(bb)
Control 传播同一 bb
叶子 setOutput("result", v) / getInput("result")
```

### 设计意图和意义

控制节点只调度，不理解业务数据。若 Sequence 传参：

- Sequence 必须知道每个孩子的类型
- 换树结构就要改传参路径
- 控制流与数据流耦合

黑板是间接寻址：写节点与读节点约定 key，控制节点无需知道。

### 交付文件

```text
levels/L5_blackboard/blackboard.hpp
levels/L5_blackboard/three_node.hpp
levels/L5_blackboard/l5_blackboard.cpp
```

### 实现顺序

1. Blackboard set/get。
2. ThreeNode 挂 bb。
3. Control 传播。
4. Set + Say demo。

### 自检

- [ ] 打印内容等于写入内容
- [ ] 缺 key / 类型错有明确错误
- [ ] 能解释为何不让 Sequence 当管道传参

### 通过后

进入 [L6](#L6)。

---

<a id="L6"></a>
## L6 · Retry 装饰器

**章节副本**：[levels/L6_retry_decorator/](levels/L6_retry_decorator/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

装饰器只有一个孩子，改变执行策略。重点 `RetryNode`：

```text
孩子失败后不立刻整段失败，而在后续 tick 再试
每次 tick 最多 tick 孩子一次
还可重试时返回 RUNNING（不要一次 for 循环试完）
```

### 需求边界

**要写：** `DecoratorBase` + `RetryNode(max_attempts)` + “前 2 次失败第 3 次成功”叶子。

**不要写：** Timeout/Inverter、真线程、完整 Port。

### 推荐设计

```text
孩子 RUNNING -> Retry RUNNING
孩子 SUCCESS -> 清 attempt，SUCCESS
孩子 FAILURE -> attempt++；未超限则复位孩子并 RUNNING；否则 FAILURE
```

### 设计意图和意义

Retry 是横切策略：开门节点只负责开门，Retry 负责再试。一次 tick 内循环重试会破坏协作式模型，并与 Sequence 书签语义冲突。

### 交付文件

```text
levels/L6_retry_decorator/decorator.hpp
levels/L6_retry_decorator/retry_node.hpp
levels/L6_retry_decorator/l6_retry.cpp
```

### 自检

- [ ] 最终 SUCCESS，孩子被执行 3 次且分布在多次 root tick
- [ ] 能解释 Decorator（单孩子）vs Control（多孩子）
- [ ] 能说明为何失败还可重试时返回 RUNNING

### 通过后

进入 [L7](#L7)。

---

<a id="L7"></a>
## L7 · halt 传播

**章节副本**：[levels/L7_halt_propagation/](levels/L7_halt_propagation/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

补齐生命周期：父节点取消或控制流结束时，RUNNING 孩子必须被通知停下。

实现：

- `ThreeNode::halt()` / `resetStatus()`
- `ControlNode::resetChildren()`、`DecoratorBase::resetChild()`
- Sequence/Fallback 终态或切换时清理
- Stateful `onHalted()` 打日志

### 需求边界

**要写：** 取消/复位传播。

**不要写：** 新业务类型、线程取消、析构自动 halt。

### 推荐设计

```text
ControlNode::resetChildren():
  每个 child：
    RUNNING -> halt()
    否则 -> setStatus(IDLE)
  index = 0
```

Demo：Sequence 内 Sleep 进入 RUNNING 后对根 `halt()`，应看到 `onHalted`。

### 设计意图和意义

`halt` ≠ `FAILURE`。失败是业务结果；halt 是取消/打断。节点对象通常长期存活，不能靠析构自动停。没有传播会出现：方案已切走，Sleep 还以为自己在跑。

### 交付文件

```text
levels/L7_halt_propagation/control_node.hpp
levels/L7_halt_propagation/l7_halt.cpp
```

### 自检

- [ ] halt 后看到 onHalted；status 回到 IDLE
- [ ] 再次 tick 会重新 onStart（若语义如此）
- [ ] 能说明 halt 与 FAILURE 区别

### 通过后

进入 [L8](#L8)。

---

<a id="L8"></a>
## L8 · 手写树里程碑

**章节副本**：[levels/L8_graduate/](levels/L8_graduate/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

整合 L0～L7，组出完整“进房间”策略树，并写设计对照。

**这是运行时核心毕业，不是完整 BehaviorTree.CPP 使用链路毕业。** 后续见 [L9](#L9)～[L14](#L14)。

建议树：

```text
MainTree (Sequence)
  EnterRoomFallback (Fallback)
    plan_A (Sequence): BatteryOK, DoorOpen, EnterRoom
    plan_B (Sequence): FindKey, Retry(FlakyOpenDoor), Sleep, EnterRoom, SetBlackboard
  SaySomething(result)
```

### 需求边界

**可用：** L1～L7 全部能力。

**不要提前发明：** XML、Factory、正式 Port remapping。本关重点是组合，不是新框架层。

### 设计意图和意义

证明每个抽象各司其职：状态契约、控制流、RUNNING、黑板、装饰器、halt。复杂策略靠小节点组合，而不是巨型 `EnterRoomNode`。

### 交付文件

```text
levels/L8_graduate/l8_graduate.cpp
levels/L8_graduate/enter_room_nodes.hpp
levels/L8_graduate/DESIGN.md
```

### 实现顺序

1. 业务叶子各做一事。
2. 搭 plan_A / plan_B。
3. Fallback + MainTree + Say。
4. 挂黑板，`tickWhileRunning`。
5. 写 DESIGN.md 映射表 + “为何异步不是多线程”。

### 自检

- [ ] `l8_graduate` 最终 SUCCESS
- [ ] DESIGN.md 至少 6 行 MiniBT ↔ BT.CPP 映射
- [ ] 能口述一次 tick 调用栈与每次 RUNNING 的原因
- [ ] 理解：改树结构目前仍要改 C++；真正分离在 L9～L14

### 通过后

进入 [L9](#L9)。勾上清单 L8，并意识到路线尚未结束。

---

<a id="L9"></a>
## L9 · Ports：节点接口与黑板映射

**章节副本**：[levels/L9_ports/](levels/L9_ports/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

把 L5“节点写死黑板 key”升级为正式端口模型：

```text
节点逻辑端口名  --remapping-->  Blackboard key
```

**一句话：** Port = 节点的形参/返回值；Groot 的“线”≠ 节点指针直连，而是两个端口绑定到同一黑板槽位。

```text
MakeMessage.text -> Blackboard["door_message"] -> SaySomething.message
```

### 需求边界

**要写：**

- `PortDirection` / `PortInfo` / `PortsList`
- `NodeConfig`（blackboard + input/output remapping）
- `providedPorts()`
- remapping 版 `getInput` / `setOutput`
- 支持常量 `"hello"` 与 `{door_message}`

**不要写：** XML、node-to-node 边表、`{=}` / 默认值可后补。

### 推荐设计

```text
NodeConfig
  blackboard
  input_ports:  message -> "hello" | "{door_message}"
  output_ports: text    -> "{door_message}"

getInput("message"):
  常量 -> convertFromString
  {key} -> blackboard.get(key)

setOutput("text", v):
  必须有 {key} remapping -> blackboard.set(key, v)
```

节点实现只认识 `message`，不认识外部 key 名。

### 设计意图和意义

L5 让数据能传；L9 让**接口稳定、实例可变**：同一 `SaySomething` 可映射到任意 key，无需改节点代码。这是 XML 属性接线、以及 Groot 画“线”的运行时真相。

错误模型：建一张 `Producer.port -> Consumer.port` 边表。BT.CPP 运行时没有这条边。

### 交付文件

```text
levels/L9_ports/ports.hpp
levels/L9_ports/node_config.hpp
levels/L9_ports/three_node.hpp
levels/L9_ports/l9_ports.cpp
# 目标 level_l9_ports
```

### 实现顺序

1. 端口方向与 manifest。
2. NodeConfig + `{key}` 识别。
3. 兼容旧构造，升级 getInput/setOutput。
4. Producer → BB → Consumer demo。
5. 同一 Consumer 映射到不同 key。

### 自检

- [ ] 同一节点可映射不同 key，无需改节点源码
- [ ] 输出/输入绑定同一 key 可传值
- [ ] 缺 remapping / 缺 key / 类型错有明确错误
- [ ] 能口述 Port、remapping、Blackboard key、Groot 线四者区别
- [ ] `l8_graduate` 仍可运行（兼容）

### 通过后

进入 [L10](#L10)。

---

<a id="L10"></a>
## L10 · Factory：注册与字符串实例化

**章节副本**：[levels/L10_factory/](levels/L10_factory/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

把 `make_shared<BatteryOK>` 改成用字符串 ID 找构造器。XML 只有标签名，没有 C++ 类型，必须先有 Factory。

### 需求边界

**要写：**

- `registerNodeType<T>("BatteryOK")`
- `registerBuilder(id, builder, manifest)`
- `instantiateTreeNode(name, id, config)`
- Registry：**builders** + **manifests**（端口模型）
- `Tree`：`tickOnce` / `tickWhileRunning`
- 注册 Sequence / Fallback / Retry / 场景叶子

**不要写：** XML。本关仍可用 C++ 手工 `addChild`，但节点由 Factory 创建。

### 推荐设计

```text
builders["BatteryOK"]  -> NodeBuilder
manifests["BatteryOK"] -> type + providedPorts()
```

builder 只构造当前节点；孩子由后续 XML 实例化层挂接。

### 设计意图和意义

Factory 把“类型系统里的类”翻译成“配置世界里的名字”。没有它，XML 标签无法落到 C++ 对象；有了它，错误可收敛在“未注册 ID”边界。

### 交付文件

```text
levels/L10_factory/bt_factory.hpp
levels/L10_factory/tree.hpp
levels/L10_factory/register_builtin_nodes.hpp
levels/L10_factory/l10_factory.cpp
# 目标 level_l10_factory
```

### 实现顺序

1. NodeBuilder 签名。
2. registerBuilder → registerNodeType。
3. 内置控制/装饰/叶子注册。
4. Tree 封装 tick 循环。
5. Factory 建节点 + 手工组树，行为对齐 L8 数据流片段。

### 自检

- [ ] 已注册 ID 创建正确类型
- [ ] 未注册 ID 报错含 ID 名
- [ ] 同一 registration ID 可多实例、不同实例名
- [ ] Factory 节点仍能用 L9 Ports

### 通过后

进入 [L11](#L11)。

---

<a id="L11"></a>
## L11 · XML：只解析为 AST

**章节副本**：[levels/L11_xml_parse/](levels/L11_xml_parse/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

XML 文本 → 中间结构（AST）。严格分开：

```text
语法正确 ≠ 节点已注册 ≠ 树能运行
```

### 最小 XML 子集

```xml
<root BTCPP_format="4" main_tree_to_execute="MainTree">
  <BehaviorTree ID="MainTree">
    <Sequence name="main">
      <BatteryOK name="battery"/>
      <SaySomething name="say" message="{door_message}"/>
    </Sequence>
  </BehaviorTree>
</root>
```

### 需求边界

**要写：** TinyXML2 解析；保存 tag、attributes、有序 children；识别 `main_tree_to_execute` 与 `BehaviorTree ID`；错误诊断。

**不要写：** 调 Factory、创建 ThreeNode、tick。`{door_message}` 仅原样保留为字符串。

### 推荐设计

```text
XmlNode { tag, attributes, children }
XmlDocumentModel { main_tree_id, behavior_trees: ID -> XmlNode }
```

Parser **禁止**硬编码 `BatteryOK` 构造逻辑。

### 设计意图和意义

若在解析器里直接 `new BatteryOK`，语法层与类型层缠死，后续无法复用 AST、无法单独测解析错误。官方也是先解析再实例化。

### 交付文件

```text
levels/L11_xml_parse/xml_ast.hpp
levels/L11_xml_parse/xml_parser.hpp
levels/L11_xml_parse/xml_parser.cpp
levels/L11_xml_parse/l11_sample.xml
levels/L11_xml_parse/l11_parse_xml.cpp
# 本关目录内链接 tinyxml2，不要引用 full/
```

### 实现顺序

1. 引入 TinyXML2。
2. 定义 AST。
3. 从字符串/文件加载。
4. 校验 root、Tree ID、主树引用。
5. demo 只打印 AST。

### 自检

- [ ] 字符串与文件均可解析
- [ ] children 顺序与 XML 一致；`{...}` 原样保留
- [ ] 非法 XML、重复 Tree ID、主树缺失有清晰错误
- [ ] demo 不偷偷创建运行时节点

### 通过后

进入 [L12](#L12)。

---

<a id="L12"></a>
## L12 · XML：递归实例化整棵树

**章节副本**：[levels/L12_xml_tree/](levels/L12_xml_tree/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

打通 L10 + L11：

```text
XML tag -> registration ID -> manifest 校验属性
  -> NodeConfig / Port remapping
  -> builder 创建节点
  -> 递归挂接 children
  -> Tree.tickWhileRunning()
```

### 需求边界

**要写：**

- 标签名 = registration ID；`name` = 实例名
- 按 manifest 把属性写入 input/output remapping
- Control 0..N 孩子；Decorator 恰好 1；Leaf 0
- `Retry num_attempts`、`Sleep ticks` 等配置端口
- `createTreeFromText()` / `createTreeFromFile()`

**不要写：** SubTree、插件、脚本、完整 BTCPP_format=4。

### 推荐设计

实例化器读 manifest 分类属性；Parser 不做分类；节点不读 XML。结构约束在建树期检查。

### 设计意图和意义

这是“描述与实现分离”的关键一跃：改 XML 即可改策略形状，业务仍由已注册 C++ 提供。属性必须进 NodeConfig，否则 Ports 形同虚设。

### 交付文件

```text
levels/L12_xml_tree/xml_instantiation.hpp
levels/L12_xml_tree/xml_instantiation.cpp
levels/L12_xml_tree/l12_enter_room.xml
levels/L12_xml_tree/l12_xml_tree.cpp
```

### 实现顺序

1. 取主 Tree AST 根。
2. 查 Factory + 校验属性。
3. 建共享 BB + NodeConfig。
4. builder 创建当前节点。
5. 递归挂孩子。
6. tick 到终态，对照 L8 行为。

### 自检

- [ ] XML 树与 L8 手写版同构且最终状态一致
- [ ] 未注册标签 / 未知 Port / 非法孩子数：建树期失败
- [ ] Retry/Sleep 配置来自 XML 且跨 tick 正确
- [ ] halt 对 XML 树仍可传播

### 通过后

进入 [L13](#L13)。

---

<a id="L13"></a>
## L13 · SubTree 与跨黑板 remapping

**章节副本**：[levels/L13_subtree/](levels/L13_subtree/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

同一树内：多端口常共享同一 BB key（L9 的“线”）。  
跨 SubTree：子树有自己的 Blackboard，边界上做 `internal -> external` 映射。

```xml
<BehaviorTree ID="MainTree">
  <Sequence>
    <MakeMessage text="{root_message}"/>
    <SubTree ID="ReportTree" message="{root_message}"/>
  </Sequence>
</BehaviorTree>

<BehaviorTree ID="ReportTree">
  <SaySomething message="{message}"/>
</BehaviorTree>
```

```text
父 BB[root_message]
  <-> SubTree(message -> root_message)
  <-> 子 BB[message]
  <-> SaySomething.message
```

### 需求边界

**要写：** 子 BB 指向父 BB；显式 remapping；同一 Tree ID 可多次实例化。

**不要写：** `_autoremap`、私有 `_` key、脚本（可后补）。

### 设计意图和意义

SubTree = **树定义复用**，不是共享同一组节点对象。子树内部只认识自己的端口名；父级 key 改名只需改边界 XML。

### 交付文件

```text
levels/L13_subtree/subtree.hpp
levels/L13_subtree/blackboard.hpp
levels/L13_subtree/l13_subtree.xml
levels/L13_subtree/l13_subtree.cpp
```

### 自检

- [ ] 子树不依赖父级 key 名
- [ ] 改父级 key 只改边界映射
- [ ] 同一子树不同 remapping 实例化两次，不共享运行状态
- [ ] 映射/递归引用错误有诊断

### 通过后

进入 [L14](#L14)。

---

<a id="L14"></a>
## L14 · 最终毕业：完整链路

**章节副本**：[levels/L14_xml_graduate/](levels/L14_xml_graduate/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

一条命令跑通 XML 版进房间，并完成端到端追踪：

```text
C++ providedPorts + registerNodeType
  -> load XML
  -> parse BehaviorTree
  -> Factory 递归实例化
  -> Port remapping
  -> Blackboard 读写
  -> tickWhileRunning
  -> halt/reset 收尾
```

### 需求边界

至少包含：Sequence/Fallback、Retry + Stateful RUNNING、一对 output/input Port、一个 SubTree remapping、可 halt 路径。

**刻意不做（非毕业条件）：** 动态插件、Groot2 ZMQ、**嵌入式脚本 / PrePostCondition**（见独立拓展线 [E0](#E0)～[E9](#E9)）、JSON 泛型 Port、Parallel/Reactive/Coroutine 全家桶。

### 设计意图和意义

真正毕业标准不是“能 parse XML”，而是能解释并运行：

```text
结构由 XML 决定
行为由注册的 C++ 节点提供
数据接口由 Ports 声明
数据实例由 Blackboard 保存
调度由 tick / RUNNING / halt 完成
```

### 交付文件

```text
levels/L14_xml_graduate/l14_enter_room.xml
levels/L14_xml_graduate/l14_xml_graduate.cpp
levels/L14_xml_graduate/test_ports.cpp
levels/L14_xml_graduate/test_factory.cpp
levels/L14_xml_graduate/test_xml.cpp
levels/L14_xml_graduate/test_subtree.cpp
# 手写基线对照 levels/L8_graduate，不要引用 full/
```

### 端到端追踪题（必须能口述）

对 `<SaySomething message="{result}"/>`：

1. `SaySomething` 已注册，manifest 声明输入 Port `message`
2. Parser 保存属性字符串 `{result}`
3. 实例化器写入 `NodeConfig.input_ports["message"]`
4. `tick()` → `getInput("message")`
5. 识别 `{result}` → 读 `Blackboard["result"]`
6. 数据来自更早的输出 Port，不是节点直连边

### 自检

- [ ] `l8_graduate` 与 `l14_xml_graduate` 最终状态一致
- [ ] 改 XML 可改策略，无需重编译框架
- [ ] 业务节点不写死对方节点名或外部 BB key
- [ ] XML/注册/Port 方向/类型/SubTree 错误有测试
- [ ] XML 树上 Retry、RUNNING、halt 语义不退化
- [ ] 能完整口述上列追踪题

### 通过后

主线完成。可选支线 [B1](#B1)～[B5](#B5)。  
若想学官方「XML 里写短脚本改黑板 / 前置条件」机制，进入**独立拓展线** [E0](#E0)（不是 L15）。

---

<a id="B1"></a>
## B1 · 极简文本组树（支线）

**回清单**：[↑](#关卡列表清单)

在 XML 前用缩进文本验证“解析 → 递归组树”逻辑，降低 TinyXML2 干扰。主线已通可跳过。

**交付建议：** `assets/b1_indent_tree.txt` + `demos/b1_indent.cpp`  
**自检：** 文本树 tick 行为与手写/XML 片段一致。

---

<a id="B2"></a>
## B2 · Timeout（支线）

按 tick 计数的 Timeout 装饰器：超时则 halt 孩子并 FAILURE。  
**交付：** `timeout_node.hpp` + demo。  
**自检：** Sleep(10) 包在 Timeout(3) 下，应看到 onHalted。

---

<a id="B3"></a>
## B3 · ReactiveSequence（支线）

每次从第一个孩子重新检查；理解与普通 Sequence 书签差异。对照官方 Reactive 语义，MiniBT 可只做教学子集。

---

<a id="B4"></a>
## B4 · 插件动态加载（支线）

共享库注册节点。主线 L10 静态注册已够用；本支线对标 BT.CPP 插件。

---

<a id="B5"></a>
## B5 · Groot2 可视化（支线）

ZeroMQ 状态上报。非最小链路必需。

---

<a id="E-scripting"></a>
# 拓展线 · 嵌入式脚本（E0～E9）

**章节总目录**：[levels/E_scripting/](levels/E_scripting/) · **回清单**：[↑](#关卡列表清单)

## 这是什么 / 不是什么

| 是 | 不是 |
|----|------|
| 主线毕业后的**独立拓展章节** | L15、主线续关、毕业条件之一 |
| 对标 BT.CPP 的 Script / `_skipIf` / `_onSuccess` | 把 Lua/Python 嵌进 MiniBT |
| 可单独从 E0 开写的练习线 | 必须改动 L0～L14 目录才能开始 |

**官方动机（一句话）：** 把「条件守卫 / 副作用赋值」从 C++ 节点抽到配置字符串里；运行时只读写 Blackboard。

**端到端目标形态：**

```text
XML: <Move _skipIf="ready" _onSuccess="n:=n+1"/>
  -> 解析属性（字符串）
  -> ParseScript 编译成 ScriptFunction
  -> executeTick 前 checkPre / 完成后 checkPost
  -> 脚本读写同一个 Blackboard
```

**建议前置：** L5；做 E7/E8 时最好已做过 L1 的 `executeTick` 与 L12/L14 的 XML。  
**对照仓库（只读）：** 同级 `BehaviorTree.CPP` 的 `scripting/`、`tree_node.cpp`、`examples/t09_scripting.cpp`。

---

<a id="E0"></a>
## E0 · 定语言边界

**本关目录**：[levels/E_scripting/E0_language/](levels/E_scripting/E0_language/) · **回清单**：[↑](#关卡列表清单)

### 本关目标

先写清「做什么 / 不做什么」，避免做成通用脚本语言。

### 需求边界

**要写：** 一页语法说明书：`levels/E_scripting/E0_language/LANGUAGE.md`。

**语言只包含：**

```text
- 类型：number / bool / string（可用 Any / variant）
- 运算：算术、比较、逻辑、三元 ?:
- 赋值：:=（新建） =（已有） += -= *= /=
- 多语句：用 ; 分隔；求值返回最后一条
- 变量名 = Blackboard 的 key
```

**明确不做：** 函数定义、循环、import、自定义类型、文件系统 / 网络 API、`eval` 宿主语言。

### 示例语法（后续各关要实现）

```text
A := 42
msg := 'hello'
ok := A > 10 && color != 2
A += 1; B := A * 2
x > 0 ? 'pos' : 'neg'
```

### 设计意图和意义

边界越窄，词法/语法/安全面越小，才能在配置加载期编译、在 tick 里便宜执行。官方 BT 脚本也是「表达式 + 赋值」，不是嵌入 Lua。

### 自检

- [ ] 能用约 10 行说明：能写什么、不能写什么、变量存在哪
- [ ] 清单里勾上 E0；进入 [E1](#E1)

---

<a id="E1"></a>
## E1 · Environment（脚本眼中的世界）

**本关目录**：[levels/E_scripting/E1_environment/](levels/E_scripting/E1_environment/)

### 本关目标

脚本不直接碰节点，只碰环境：

```cpp
struct Environment {
  std::shared_ptr<Blackboard> vars;
  std::shared_ptr<std::unordered_map<std::string, int>> enums; // E9 再填
};
```

可复用主线 L5 的 Blackboard 思路，但在本关目录内自备一份，勿改 L5 目录。

### 需求边界

**要写：** `Any`（或 `std::any` 包装）、`Blackboard::set/get`、`Environment`。

**不要写：** tokenizer、parser、节点钩子。

### 自检

- [ ] `bb->set("A", 42)` 后能读回
- [ ] 缺 key 有明确错误
- [ ] 进入 [E2](#E2)

---

<a id="E2"></a>
## E2 · Tokenizer

**本关目录**：[levels/E_scripting/E2_tokenizer/](levels/E_scripting/E2_tokenizer/)

### 本关目标

`std::string` → `vector<Token>`。

建议 `TokenType`：`Number` / `String` / `Boolean` / `Identifier`、算术与比较、`AmpAmp` / `PipePipe` / `Bang`、`ColonEqual` / `Equal` / 复合赋值、`Question` / `Colon`、`DotDot`、括号、`Semicolon`、`EndOfInput`。

### 注意点

- 多字符算符最长匹配：`:=` 先于 `:`，`==` 先于 `=`，`&&` `||` `+=` 等同理
- 字符串字面量风格固定一种（推荐 `'...'`，对齐 BT）
- Token 带 `pos`，方便报错

### 验收输入

```text
A:=42; B>3 && true
```

应依次得到：Ident / ColonEqual / Number / Semicolon / Ident / Greater / Number / AmpAmp / Boolean / EOF。

### 自检

- [ ] 上列用例 token 序列正确
- [ ] 非法字符有带位置的错误
- [ ] 进入 [E3](#E3)

---

<a id="E3"></a>
## E3 · AST + 手写求值（先别写 parser）

**本关目录**：[levels/E_scripting/E3_ast/](levels/E_scripting/E3_ast/)

### 本关目标

定义 `ExprBase::evaluate(Environment&)`，手写 AST 测通求值，确认「语义」先于「语法」。

至少实现：

- `ExprLiteral` / `ExprName`
- 一元：`-` `!`
- 二元：`+ - * /`、比较、`&&` `||`

### 推荐设计

```text
ExprName: 先查 enums（可空），再查 vars
数值比较 / 逻辑：统一用 double 或明确 cast 规则
```

### 自检

- [ ] 手写 `A+1`（A=41）得到 42
- [ ] `1<2 && 3>1` 为真
- [ ] 进入 [E4](#E4)

---

<a id="E4"></a>
## E4 · Pratt 解析器（本拓展线核心关）

**本关目录**：[levels/E_scripting/E4_pratt_parser/](levels/E_scripting/E4_pratt_parser/)

### 本关目标

用 **binding power** 把 token 流变成 AST。不要堆一串 `parseAdd/parseMul` 递归下降（可作对照，但本关目标是 Pratt）。

```text
parseExpr(min_bp):
  left = parsePrefix()
  while leftBP(peek) >= min_bp:
    op = advance()
    right = parseExpr(...)
    left = makeBinary(left, op, right)
  return left
```

### 推荐优先级（对齐 BT）

```text
:= = += ...     最松
?:
||
&&
比较
+ - ..
* /
一元 - !        最紧
```

### 验收用例

| 输入 | 期望 |
|------|------|
| `1+2*3` | 7 |
| `(1+2)*3` | 9 |
| `1<2 && 3>1` | true |
| `!false` | true |
| `a>0?10:20`（a=1） | 10 |

### 设计意图和意义

表达式语言的难点是优先级与结合性。Pratt 用一张表扩展算符，后续加 `..` 或位运算不必重写整棵解析树。

### 自检

- [ ] 上表全部通过
- [ ] 进入 [E5](#E5)

---

<a id="E5"></a>
## E5 · 赋值与多语句

**本关目录**：[levels/E_scripting/E5_assignment/](levels/E_scripting/E5_assignment/)

### 本关目标

| 算符 | 语义 |
|------|------|
| `:=` | 创建或覆盖 |
| `=` | 仅已存在变量可写 |
| `+=` 等 | 读旧值再写回 |

多语句：`parseStatements` 用 `;` 分隔；求值时前面丢弃结果，**返回最后一条**（条件脚本靠它当 bool）。

### 自检

- [ ] `A:=1; B:=A+2; B` → 3，且 BB 有 A、B
- [ ] `x = 1` 在 x 不存在时失败
- [ ] 进入 [E6](#E6)

---

<a id="E6"></a>
## E6 · ParseScript：编译成闭包

**本关目录**：[levels/E_scripting/E6_script_function/](levels/E_scripting/E6_script_function/)

### 本关目标

```cpp
using ScriptFunction = std::function<Any(Environment&)>;
Expected<ScriptFunction> ParseScript(const std::string& script);
```

原则：

- **加载/建树时 parse 一次**
- **运行时只调用闭包**
- 错误信息带上原始脚本字符串

### 最小可运行里程碑

```cpp
auto set  = ParseScript("A:=1; msg:='hi'").value();
auto cond = ParseScript("A > 0 && msg == 'hi'").value();
set(env);
assert(cond(env)./*真*/);
```

### 设计意图和意义

这是与「每次 tick 重新解析字符串」的分水岭。官方 `ParseScript` 同样返回可调用对象，供节点缓存。

### 自检

- [ ] 同一 `ScriptFunction` 调用 1000 次不再 tokenize
- [ ] 空脚本 / 语法错误返回可读错误
- [ ] 进入 [E7](#E7)

---

<a id="E7"></a>
## E7 · 挂到宿主 executeTick

**本关目录**：[levels/E_scripting/E7_tick_hooks/](levels/E_scripting/E7_tick_hooks/)

### 本关目标

在**本拓展目录**实现最小节点模型（可抄主线思路，勿改 L1～L14 源文件）：

```text
executeTick:
  if pre 短路 -> 直接返回状态
  else status = tick()
  if 终态 -> 跑 post 脚本
```

Pre（建议顺序）：`_failureIf` → `_successIf` → `_skipIf` → `_while`  
Post：`_onSuccess` / `_onFailure` / `_onHalted` / `_post`

要点（对齐官方）：

- `_failureIf` / `_successIf` / `_skipIf`：**仅 IDLE（或 SKIPPED）时检查一次**
- `_while`：IDLE 与 RUNNING 都查；RUNNING 中变假则 halt → SKIPPED

另做：

- `ScriptNode`：执行 `code`，写 BB，返回 SUCCESS
- `ScriptCondition`：表达式为真 SUCCESS，否则 FAILURE

### 自检

- [ ] `_skipIf="done"` 且 `done=true` 时不进 `tick_impl`
- [ ] `_onSuccess="n:=n+1"` 在 SUCCESS 后 BB 增加
- [ ] 进入 [E8](#E8)

---

<a id="E8"></a>
## E8 · 配置文件生效（完整链路）

**本关目录**：[levels/E_scripting/E8_config_integration/](levels/E_scripting/E8_config_integration/)

### 本关目标

打通「字符串属性 → 编译 → tick 生效」：

```xml
<Sequence>
  <Script code="A:=0; ready:=false"/>
  <Work
      _skipIf="ready"
      _while="A &lt; 10"
      _onSuccess="ready:=true"/>
</Sequence>
```

### 推荐流水线

```text
1. XML 读保留属性 → NodeConfig 里仍是 string
2. Factory/建节点时 ParseScript → 填 pre/post 数组
3. 枚举表挂到 Environment
4. tick 时用同一 Blackboard 求值
```

**不要**在 XML parser 里执行脚本；parser 只保存字符串。

### 设计意图和意义

与主线 L11/L12 同一哲学：描述层只存文本，实现层编译，运行层执行。脚本是「配置世界」的又一种载荷。

### 自检

- [ ] 只改 XML/配置字符串，行为改变，无需改业务 `tick()` 源码
- [ ] 非法脚本在**建树期**失败，而不是第一次 tick 才炸
- [ ] 进入 [E9](#E9)

---

<a id="E9"></a>
## E9 · 加固

**本关目录**：[levels/E_scripting/E9_hardening/](levels/E_scripting/E9_hardening/)

### 本关目标（按优先级）

1. `registerScriptingEnum`：标识符先查枚举表再查 BB  
2. `ValidateScript`：只 parse 不执行  
3. `ScriptNode` 缓存：`code` 未变不重新 Parse  
4. 错误带 `pos` + 原始脚本 + 变量名  
5. 测试：词法 / 优先级 / 赋值 / 短路 / pre-post 时序  
6. 安全：不开放宿主 `eval`、文件、网络

### 对照官方

| 本拓展 | BehaviorTree.CPP |
|--------|------------------|
| `tokenize` | `script_tokenizer.cpp` |
| Pratt `Parser` | `script_parser.cpp` |
| `Expr*` | `scripting/operators.hpp` |
| `ParseScript` | `script_parser.hpp` |
| `Environment` | `Ast::Environment` |
| pre/post 钩子 | `TreeNode::executeTick` |
| XML 属性编译 | `xml_parsing` + `bt_factory` |
| 示例 | `examples/t09_scripting.cpp` |

### 自检

- [ ] 有一组固定测试可回归
- [ ] 能口述：配置字符串 → 闭包 → Blackboard 的路径
- [ ] 拓展线清单 E0～E9 全部勾选

### 通过后

拓展线完成。可回到支线 [B1](#B1)～[B5](#B5)，或对照官方 `t09_scripting` 做差异笔记。  
**不要**把本线成果强行改编号塞进主线 L15；若要并入 `full/`，单独开集成任务。

---

## 设计哲学（闯关时反复对照）

1. 编排与行为分离  
2. 统一状态契约  
3. 父节点调度孩子  
4. 协作式异步（RUNNING）  
5. 数据走黑板；接口走 Port  
6. 装饰器管横切  
7. 结构描述（XML）与实现（C++）分离  
8. 组合优于巨型状态机  

---

## 与官方库对照

| MiniBT | BehaviorTree.CPP |
|--------|------------------|
| `ThreeNode` / `TreeNode` | `BT::TreeNode` |
| `SequenceNode` | `BT::SequenceNode` |
| `FallbackNode` | `BT::FallbackNode` |
| `StatefulAction` | `BT::StatefulActionNode` |
| `Blackboard` | `BT::Blackboard` |
| `RetryNode` | `BT::RetryNode` / `RetryUntilSuccessful` |
| `NodeConfig` + `{key}` | Ports remapping |
| `BehaviorTreeFactory` | `BT::BehaviorTreeFactory` |
| Mini XML parser | TinyXML2 + `XMLParser` |
| `SubTree` 显式映射 | `SubTree` remapping |
| 手写组树（L8） | XML + Factory（L14） |
| `tickWhileRunning` | `BT::Tree::tickWhileRunning` |
| 拓展线 E：脚本 / pre-post（非主线） | `Script` / `ScriptCondition` / `_skipIf` 等 |

官方参考：同级 `BehaviorTree.CPP/examples/`（尤其 t01、t02、t06；脚本见 t09）。

---

## 防跑偏

- L0～L8 不要提前写 XML；L9～L14 严格按 Port → Factory → Parser → 实例化 → SubTree → 总装
- 不要把 Groot 的线实现成节点之间的直接指针
- 不要在 XML parser 里硬编码业务节点构造
- 不要一开始追求完整 BTCPP_format=4、插件、脚本、ZMQ
- **脚本机制走拓展线 E0～E9，不要做成 L15，也不要在主线关卡里提前实现**
- L4 禁止用 `sleep_for` 代替 tick 计数
- 每关只新增本关文件；通过自检再开下一关
- L8 通过后请勾清单并继续 L9，勿误以为路线结束
- 拓展线每关使用 `levels/E_scripting/E*_*/` 独立目录；不要改已过关的 E 关或 L0～L14 目录
