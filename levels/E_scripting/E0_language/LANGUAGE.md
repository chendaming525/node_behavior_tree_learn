# MiniBT 脚本语言说明书（E0 模板）

> E0 交付：把本文件完善成你自己的最终语言规格。

## 定位

这是一门嵌入式表达式语言，只负责读写 Blackboard，供 Script 节点和 pre/post 条件使用。它不是通用编程语言。

## 类型

| 类型 | 例子 |
|------|------|
| number | `42`、`3.14` |
| bool | `true`、`false` |
| string | `'hello'` |

比较和逻辑运算必须明确真假转换规则。建议 `0` / `false` 为假，非零为真。

## 变量

- 标识符对应 Blackboard key，例如 `A`、`ready`、`msg`。
- E9 可增加枚举常量，例如 `RED`、`THE_ANSWER`。

## 语句与表达式

```text
A := 42
A = 10
A += 1
msg := 'hello'
ok := A > 10 && ready
A += 1; B := A * 2
x > 0 ? 'pos' : 'neg'
```

## 运算符优先级（从松到紧）

1. 赋值：`:=` `=` `+=` `-=` `*=` `/=`
2. 三元：`? :`
3. 逻辑或：`||`
4. 逻辑与：`&&`
5. 比较：`==` `!=` `<` `>` `<=` `>=`
6. 加减 / 拼接：`+` `-` `..`
7. 乘除：`*` `/`
8. 一元：`-` `!`

## 不做的事

- 函数定义、循环、import
- 文件、网络、线程
- 宿主 C++ / Python `eval`
- 自定义结构体语法

## 与宿主的约定

| 场景 | 脚本角色 |
|------|----------|
| `Script code` | 执行副作用，节点返回 SUCCESS |
| `ScriptCondition` | 最后一条表达式作为条件 |
| `_skipIf` 等 | 结果转为 bool，由 `executeTick` 解释 |
| `_onSuccess` 等 | 执行副作用，返回值忽略 |

## 编译模型

```text
配置加载期：ParseScript(string) -> ScriptFunction
运行期：ScriptFunction(Environment{blackboard, enums}) -> ScriptValue
```
