# E9 - 加固与毕业

## 本关目标

把“能跑”的脚本机制补成可诊断、可缓存、可回归的教学实现。

## 加固项

1. `registerScriptingEnum(name, value)`
2. `ValidateScript`：只解析，不执行
3. Script 节点缓存：`code` 未变化不重新编译
4. 错误包含位置、原始脚本、变量名
5. 固定测试矩阵
6. 明确安全边界

## 枚举查找顺序

```text
ExprName
  -> 先查 Environment.enums
  -> 再查 Environment.vars
  -> 均不存在则报错
```

## 测试矩阵

| 类别 | 至少覆盖 |
|------|----------|
| Tokenizer | 最长匹配、字符串、非法字符 |
| Parser | 优先级、括号、三元、错误位置 |
| Runtime | 类型错误、缺变量、除零 |
| Assignment | `:=` / `=` / 复合赋值 |
| Tick | pre 短路、`_while`、post 时序 |
| XML | 属性转义、建树期校验 |
| Cache | 相同 code 只解析一次 |

## 安全边界

- 不开放文件、网络、线程和 shell。
- 不调用宿主语言 `eval`。
- 运行期仅通过 `Environment` 接触 Blackboard 和枚举。
- 如果未来增加函数，必须采用白名单注册。

## 文件

| 文件 | 职责 |
|------|------|
| `scripting_enums.hpp` | 枚举注册表 |
| `script_tests.cpp` | 完整回归测试 |
| `DESIGN.md` | MiniBT 与 BehaviorTree.CPP 对照 |
| `e9_graduate.cpp` | 最终示例 |

## 官方代码对照

| MiniBT 拓展 | BehaviorTree.CPP |
|-------------|------------------|
| Tokenizer | `src/script_tokenizer.cpp` |
| Pratt Parser | `src/script_parser.cpp` |
| AST | `scripting/operators.hpp` |
| Environment | `Ast::Environment` |
| tick 钩子 | `TreeNode::executeTick` |
| XML 编译 | `xml_parsing.cpp` + `bt_factory.cpp` |

## 验收

- [ ] 全部固定测试通过。
- [ ] 能口述“XML → 闭包 → tick → Blackboard”。
- [ ] 能解释为何不能每 tick 解析脚本。
- [ ] 能说明 E 线不是主线 L15。
- [ ] 能指出当前实现与官方库的差异。

通过后，拓展线 E0～E9 完成。
