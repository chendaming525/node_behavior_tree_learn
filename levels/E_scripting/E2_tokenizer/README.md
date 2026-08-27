# E2 - Tokenizer 词法分析

## 本关目标

把脚本字符串切成带类型和位置的 Token 序列。

```text
A:=42; B>3 && true
  ↓
Identifier ColonEqual Number Semicolon Identifier Greater Number AmpAmp Boolean EOF
```

## 边界

要写：

- `TokenType`
- `Token { type, text, position }`
- `tokenize(source)`
- 数字、字符串、布尔、标识符、运算符、括号和分号

不要写：

- AST 与表达式求值
- 运算符优先级
- Blackboard 赋值

## 关键规则

- 使用最长匹配：`:=` 先于 `:`，`==` 先于 `=`
- 同理处理 `!=`、`<=`、`>=`、`&&`、`||`、复合赋值
- 字符串统一采用 `'...'`
- 每个 Token 保存源码位置，错误必须带位置

## 文件

| 文件 | 职责 |
|------|------|
| `token.hpp` | Token 类型与结构 |
| `tokenizer.hpp` | Tokenizer 接口 |
| `tokenizer.cpp` | 扫描实现 |
| `e2_tokenizer.cpp` | 打印 Token 的 demo / 测试 |
| `CMakeLists.txt` | 目标 `level_e2_tokenizer` |

空架子已补全：空白、数字、标识符、字符串、最长匹配运算符、非法字符 `Error`。

## 运行

先勾选 `MINI_BT_LEVEL_E_SCRIPTING` 和 `MINI_BT_LEVEL_E2`，再 Configure。

```bash
cmake -S levels -B build_levels
cmake --build build_levels --config Release --target level_e2_tokenizer
```

## 实现顺序

1. 定义 Token 类型。
2. 跳过空白。
3. 扫描数字、标识符与关键字。
4. 扫描字符串。
5. 按最长匹配扫描运算符。
6. 追加 `EndOfInput`。

## 验收

- [x] 示例输入产生正确 Token 序列。
- [x] `=` 与 `==`、`:` 与 `:=` 不混淆。
- [x] 未闭合字符串和非法字符报告准确位置。
- [x] Tokenizer 不执行任何表达式。

通过后进入 [E3](../E3_ast/)。
