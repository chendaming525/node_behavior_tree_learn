# E6 - ParseScript 编译成闭包

## 本关目标

把解析后的 AST 封装成可重复执行的 `ScriptFunction`。

```cpp
using ScriptFunction = std::function<ScriptValue(Environment&)>;
```

## 核心接口

```cpp
Expected<ScriptFunction> ParseScript(const std::string& script);
Expected<ScriptValue> ParseScriptAndExecute(
    Environment& env, const std::string& script);
Result ValidateScript(const std::string& script);
```

`ValidateScript` 可在 E9 完善，本关先实现 `ParseScript`。

## 边界

要写：

- tokenize + parse 的统一入口
- AST 捕获到闭包
- 多语句顺序求值
- 原始脚本上下文错误

不要写：

- 每次闭包执行时重新 tokenize
- pre/post 节点钩子
- XML

## 文件

| 文件 | 职责 |
|------|------|
| `script_parser.hpp` | 公共 API |
| `script_parser.cpp` | 编译闭包与错误包装 |
| `e6_script_function.cpp` | 重复执行测试 |

## 最小里程碑

```cpp
auto set = ParseScript("A:=1; msg:='hi'").value();
auto cond = ParseScript("A>0 && msg=='hi'").value();
set(env);
assert(cond(env).asBool());
```

## 验收

- [ ] 同一个闭包调用 1000 次不会重新 tokenize。
- [ ] 多语句返回最后一条结果。
- [ ] 空脚本和语法错误返回可读错误。
- [ ] 运行期错误包含原始脚本。

通过后进入 [E7](../E7_tick_hooks/)。
