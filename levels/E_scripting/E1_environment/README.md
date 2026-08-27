# E1 - 脚本运行环境

## 本关目标

建立脚本与宿主系统之间唯一的数据边界：`Environment`。

```cpp
struct Environment
{
  std::shared_ptr<Blackboard> vars;
  std::shared_ptr<EnumsTable> enums;
};
```

## 边界

要写：

- `ScriptValue`：可保存 number / bool / string
- `Blackboard::set/get/contains`
- `Environment`
- 缺 key 和类型错误诊断

不要写：

- Tokenizer、AST、Parser
- XML 与节点执行钩子
- 文件或网络能力

## 核心理解

脚本不直接持有行为树节点，也不知道 XML。所有变量读写都经过：

```text
脚本变量 A <-> Environment.vars <-> Blackboard["A"]
```

## 文件

| 文件 | 职责 |
|------|------|
| `script_value.hpp` | 动态值与类型转换 |
| `blackboard.hpp` | 本拓展线的共享变量表 |
| `environment.hpp` | 组合 Blackboard 与枚举表 |
| `main.cpp` | 最小 demo / 自检 |

## 运行

```bash
cmake -S levels -B build_levels
cmake --build build_levels --config Release --target level_e1_environment
```

开关：`MINI_BT_LEVEL_E_SCRIPTING=ON` 且 `MINI_BT_LEVEL_E1=ON`。

## 实现顺序

1. 用 `std::variant` 或受控的 `std::any` 实现值类型。
2. 实现 Blackboard 的写入、读取和存在性检查。
3. 创建 `Environment`。
4. 写入 `A=42`、`msg='hello'` 后读回。
5. 验证缺 key 与类型不匹配错误。

## 验收

- [ ] number / bool / string 均可保存并读取。
- [ ] 缺 key 时错误包含变量名。
- [ ] ScriptValue 的转换规则明确。
- [ ] 本关没有出现 Parser 或 XML。

通过后进入 [E2](../E2_tokenizer/)。
