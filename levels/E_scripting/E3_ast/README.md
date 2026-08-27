# E3 - AST 与手写求值

## 本关在整条线里的位置

```text
E2 Tokenizer：字符串 → Token 列表
E3 AST（本关）：C++ 手写树 → evaluate → ScriptValue
E4 Parser：Token 列表 → 自动长出这棵树
```

本关**故意不写 Parser**。你用 `make_shared` 直接拼出树，证明：  
「树长什么样、怎么算」可以先定死；「文本怎么变成树」下一关再做。

---

## 一句话目标

实现一套表达式 AST：每棵子树都能 `evaluate(Environment&)`，返回 `ScriptValue`。

人眼里的 `A + 1`，在机器里是一棵树：

```text
      ExprBinary(+)
       /         \
 ExprName(A)   ExprLiteral(1)
```

对 Blackboard 设 `A=41` 后调用根节点 `evaluate`，应得 `42`。

---

## 边界

**要做：**

| 节点 | 含义 | 例子 |
|------|------|------|
| `ExprLiteral` | 常量 | `42`、`true`、`'hi'` |
| `ExprName` | 读黑板变量 | `A` |
| `ExprUnary` | 一元 `-` `!` | `-A`、`!ok` |
| `ExprBinary` | 二元运算 | `A+1`、`1<2`、`a&&b` |

**不要做：**

- 从字符串 / Token 解析（那是 E4）
- `:=` 赋值、多语句 `;`（那是 E5）
- XML、Script 节点、pre/post（那是 E7/E8）

**环境：** 本关目录内自备（或复制）E1 的 `ScriptValue` / `Blackboard` / `Environment`，**不要** `#include` E1 目录或 `full/`。

---

## 核心理解

1. **AST = 语义。** 节点只知道「我是加、我是变量名」，不知道源码长什么样。  
2. **求值 = 递归。** 二元节点先算左右子树，再按运算符合并。  
3. **变量 = 查表。** `ExprName` 只做 `env.vars->get(name)`，找不到就抛错并带上变量名。  
4. **真假规则要先约定。** 建议：`false` / 数字 `0` 为假，其余数字非 0 为真；逻辑运算结果用 `bool` 或 `1.0/0.0` 二选一，demo 里写死并测通。

---

## 推荐类草图（按这个写就行）

```cpp
struct ExprBase {
  virtual ~ExprBase() = default;
  virtual ScriptValue evaluate(Environment& env) const = 0;
};
using ExprPtr = std::shared_ptr<ExprBase>;

struct ExprLiteral : ExprBase {
  ScriptValue value;
  ScriptValue evaluate(Environment&) const override { return value; }
};

struct ExprName : ExprBase {
  std::string name;
  ScriptValue evaluate(Environment& env) const override;
  // env.vars->get(name)；缺 key → throw，信息里含 name
};

enum class UnaryOp { Negate, Not };
struct ExprUnary : ExprBase {
  UnaryOp op;
  ExprPtr rhs;
  ScriptValue evaluate(Environment& env) const override;
};

enum class BinaryOp {
  Add, Sub, Mul, Div,
  Eq, Ne, Lt, Gt, Le, Ge,
  And, Or
};
struct ExprBinary : ExprBase {
  ExprPtr lhs;
  BinaryOp op;
  ExprPtr rhs;
  ScriptValue evaluate(Environment& env) const override;
};
```

辅助工厂（可选，demo 更好写）：

```cpp
ExprPtr lit(double x);
ExprPtr lit(bool b);
ExprPtr lit(const char* s);
ExprPtr name(std::string n);
ExprPtr unary(UnaryOp op, ExprPtr e);
ExprPtr binary(ExprPtr l, BinaryOp op, ExprPtr r);
```

---

## 手工组树怎么测（本关唯一入口）

**不要**写 `tokenize("A+1")`。在 `e3_ast.cpp` 里直接拼：

```cpp
// A + 1
auto env = makeEnvironment();
env.vars->set("A", ScriptValue(41.0));

ExprPtr ast = binary(name("A"), BinaryOp::Add, lit(41.0)); // 错例：右边应是 1
ast = binary(name("A"), BinaryOp::Add, lit(1.0));

ScriptValue v = ast->evaluate(env);
// v.asNumber() == 42
```

`1 < 2 && 3 > 1` 对应：

```text
            &&
         /      \
       <          >
      / \        / \
     1   2      3   1
```

```cpp
auto left  = binary(lit(1.0), BinaryOp::Lt, lit(2.0));
auto right = binary(lit(3.0), BinaryOp::Gt, lit(1.0));
auto ast   = binary(left, BinaryOp::And, right);
// evaluate → true
```

---

## 运算语义（最小约定）

| 运算 | 行为 |
|------|------|
| `+ - * /` | 两边当 number；`/` 除数为 0 → 明确错误 |
| `== != < > <= >=` | 两边同为 number，或同为 string；混类型 → 错误 |
| `&& \|\|` | 先把操作数转成 bool，再逻辑运算 |
| `-x` | number 取负 |
| `!x` | 逻辑非 |

本关**可以不做**字符串拼接 `..`、赋值；留给后面。

---

## 交付文件

```text
levels/E_scripting/E3_ast/
  expression.hpp      # ExprBase + Literal/Name/Unary/Binary
  operators.hpp/.cpp  # 可选：把 evaluate 实现拆出去
  script_value.hpp    # 从 E1 拷贝（本关自洽）
  blackboard.hpp
  environment.hpp/.cpp
  e3_ast.cpp          # 手工组树 + 断言/打印
  CMakeLists.txt      # level_e3_ast
  README.md
```

开关：`MINI_BT_LEVEL_E_SCRIPTING` + `MINI_BT_LEVEL_E3`。

空架子已放好：树在 `e3_ast.cpp` 里已经手拼好，你只改 `expression.hpp` 里每个 `evaluate` 的 TODO。

```bash
cmake -S levels -B build_levels -DMINI_BT_LEVEL_E_SCRIPTING=ON -DMINI_BT_LEVEL_E3=ON
cmake --build build_levels --config Release --target level_e3_ast
.\build_levels\E_scripting\E3_ast\Release\level_e3_ast.exe
```

填完后应打印 `A+1 = 42` 和 `1<2 && 3>1 = true`。

---

## 实现顺序（按天也不为过）

1. 拷贝 E1 的值类型与 Environment，编过空 `main`。  
2. `ExprLiteral`：`evaluate` 直接返回常量。  
3. `ExprName`：读写黑板；缺变量抛错且信息含名字。  
4. `ExprBinary` 只做 `+`，手写 `A+1` 得到 42。  
5. 补 `- * /` 与除零。  
6. 补比较与 `&&` `||`、一元 `-` `!`。  
7. 写失败用例：未定义变量、`1+'a'`、`1/0`。

---

## 验收清单

- [ ] `A=41`，手写树 `A+1` → `42`
- [ ] 手写树 `1<2 && 3>1` → true
- [ ] `ExprName("missing")` 抛错，信息含 `missing`
- [ ] `1/0` 或非法类型组合有明确错误（不要 silent wrong）
- [ ] **没有任何** `tokenize` / 字符串解析入口
- [ ] 本关只依赖本目录文件

通过后进入 [E4](../E4_pratt_parser/)：用 Pratt 把 Token 自动变成你在本关定义的这些 `Expr*`。

---

## 对照官方（只读）

BehaviorTree.CPP：`include/behaviortree_cpp/scripting/operators.hpp`  
里面的 `ExprLiteral` / `ExprName` / `ExprUnaryArithmetic` / `ExprBinaryArithmetic` 就是本关的完整版参考。E3 做教学子集即可。
