# MiniBT

用最小 C++ 实现，学透 BehaviorTree.CPP 的核心思想（可脱离 Cursor 自学）。

## 你应该怎么读

1. **[QUEST.md](QUEST.md)** — 主手册：开头有可跳转关卡清单；每关含目标、需求边界、设计意图、交付文件、实现顺序、自检  
2. **[levels/](levels/)** — **按章节深入**：每章可勾选、可单独构建运行  
3. **[full/](full/)** — 完整集成工程（回归 / 最终形态）  
4. **[DESIGN.md](DESIGN.md)** — 与官方 BT.CPP 的概念映射

不要只扫 `levels` 里的 `.hpp`：先读该章 **README**，再打开代码对照。

## 目录

```text
mini_bt/
  QUEST.md
  DESIGN.md
  levels/             ← 分章学习；每关自己的 CMakeLists.txt
    CMakeLists.txt    ← 勾选 MINI_BT_LEVEL_L0 … L14
    L0_environment/ …
    E_scripting/      ← 拓展线：嵌入式脚本（独立章节，非 L15）
  full/               ← 完整集成工程
  CMakeLists.txt      ← MINI_BT_BUILD_FULL(默认 OFF) / MINI_BT_BUILD_LEVELS(默认 ON)
```

## 章节速查（设计问题 → 哪一章）

| 你想搞懂的问题 | 去读 |
|----------------|------|
| 节点状态契约 | [L0](levels/L0_environment/) |
| 为何要 ThreeNode？tick 与 executeTick？为何只实现 tick？ | [L1](levels/L1_sync_leaves/) |
| Sequence 书签与顺序编排 | [L2](levels/L2_sequence/) |
| Fallback 备选与组合 | [L3](levels/L3_fallback/) |
| 协作式异步 / Stateful / 非多线程 | [L4](levels/L4_stateful_async/) |
| 为何要黑板？数据怎么传？ | [L5](levels/L5_blackboard/) |
| Retry 为何每次只 tick 一次并返回 RUNNING？ | [L6](levels/L6_retry_decorator/) |
| halt 是什么？父子如何一起停？ | [L7](levels/L7_halt_propagation/) |
| 手写总装进房间树 | [L8](levels/L8_graduate/) |
| Port、黑板与 Groot“连线”是什么关系？ | [L9](levels/L9_ports/) |
| XML 标签如何找到 C++ 节点？ | [L10](levels/L10_factory/) |
| 如何只解析 XML、暂不构造对象？ | [L11](levels/L11_xml_parse/) |
| 如何从 XML 递归实例化并运行整棵树？ | [L12](levels/L12_xml_tree/) |
| SubTree 如何跨父子黑板传数据？ | [L13](levels/L13_subtree/) |
| 如何打通注册、XML、Ports、Blackboard 与 tick？ | [L14](levels/L14_xml_graduate/) |
| 如何做嵌入式脚本 / `_skipIf` / `_onSuccess`？（拓展，非 L15） | [E 拓展线](levels/E_scripting/) |

L8 是“手写树运行时核心”的里程碑；L14 才是完整链路毕业。  
脚本机制在**独立拓展线 E0～E9**，不接在主线后面。

## 构建

### 关卡 levels/（默认，推荐学习时用）

```bash
cmake -S . -B build
# 或直接：
cmake -S levels -B build_levels
# cmake-gui 里勾选 MINI_BT_LEVEL_Lx
cmake --build build_levels --config Release --target level_l9_ports
.\build_levels\L9_ports\Release\level_l9_ports.exe
```

详见 [levels/README.md](levels/README.md)。

### 完整工程 full/（默认关闭，单独管理）

根工程里 `MINI_BT_BUILD_FULL` 默认为 **OFF**。需要时单独打开：

```bash
# 方式 A：只构建 full/
cmake -S full -B full/build
cmake --build full/build --config Release
.\full\build\Release\l8_graduate.exe

# 方式 B：从仓库根显式打开
cmake -S . -B build -DMINI_BT_BUILD_FULL=ON -DMINI_BT_BUILD_LEVELS=OFF
cmake --build build --config Release
.\build\full\Release\l8_graduate.exe
```
