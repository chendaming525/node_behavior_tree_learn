# L6 - Retry Decorator

## 本关目标

实现第一个装饰器节点 `RetryNode`：它只有一个孩子，可以在孩子失败时重试。

## 边界

要写：

- `DecoratorBase`
- `RetryNode`
- 一个失败若干次后成功的叶子

不要写：

- 多孩子装饰器
- XML 配置
- SubTree

## 核心规则

```text
child SUCCESS -> Retry SUCCESS
child RUNNING -> Retry RUNNING
child FAILURE and attempts left -> reset child, Retry RUNNING
child FAILURE and no attempts -> Retry FAILURE
```

## 文件

| 文件 | 职责 |
| --- | --- |
| `decorator.hpp` | 单孩子节点基类 |
| `retry_node.hpp` | 重试规则 |
| `l6_retry.cpp` | flaky action demo |

## 运行

```bash
cmake --build build_levels --config Release --target level_l6_retry
```

## 验收

- 能解释 Decorator 和 Control 的区别。
- Retry 能跨 tick 记住尝试次数。
- 孩子失败后能被复位再执行。
