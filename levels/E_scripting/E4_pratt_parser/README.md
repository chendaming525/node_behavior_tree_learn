# E4 - Pratt 表达式解析器

## 本关目标

把 E2 的 Token 流解析成 E3 的 AST，并正确处理优先级、结合性和括号。

## 推荐优先级

从松到紧：

```text
赋值
三元 ?:
||
&&
比较
+ - ..
* /
一元 - !
```

赋值将在 E5 完成，本关可先预留结合力。

## 边界

要写：

- `parsePrefix()`：字面量、变量、括号、一元运算
- `parseExpr(min_binding_power)`
- 二元运算符结合力表
- 三元表达式
- 语法错误位置

不要写：

- 多语句执行
- 节点执行钩子
- XML

## 核心算法

```text
parseExpr(min_bp):
  left = parsePrefix()
  while leftBP(peek) >= min_bp:
    op = advance()
    right = parseExpr(rightBP(op))
    left = makeExpression(left, op, right)
  return left
```

## 文件

| 文件 | 职责 |
|------|------|
| `parser.hpp` | Parser 接口 |
| `parser.cpp` | Pratt 算法 |
| `e4_parser.cpp` | 优先级与结合性测试 |

## 验收

- [ ] `1+2*3` 得到 7。
- [ ] `(1+2)*3` 得到 9。
- [ ] `1<2 && 3>1` 得到 true。
- [ ] `!false` 得到 true。
- [ ] `a>0?10:20` 在 `a=1` 时得到 10。
- [ ] 缺右括号等错误包含源码位置。

通过后进入 [E5](../E5_assignment/)。
