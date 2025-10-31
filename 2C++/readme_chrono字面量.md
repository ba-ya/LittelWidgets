`chrono` 是 **“chronology”** 或 **“chronometric”** 等与“时间”相关的英文单词的缩写或词根。

```cpp
void blink_led(milliseconds time_to_blink) {
    // ...
}

void use() {
    blink_led(1500ms);
}
```

这是 **C++14 及以上版本**中使用 **`std::chrono` 时间库** 的典型用法，它让代码**更安全、更清晰、更易读**。

------

## ✅ 一、如何实现？—— 使用 `std::chrono`

### 1. 包含头文件

```cpp
#include <chrono>
```

### 2. 使用字面量（Literals）

C++14 提供了时间字面量，需要 `using namespace`：

```cpp
using namespace std::chrono_literals;  // 必须写这行！

blink_led(1500ms);   // ✅ 正确
blink_led(2s);       // 2 秒
blink_led(5min);     // 5 分钟
blink_led(100us);    // 100 微秒
```

如果你不写 `using namespace std::chrono_literals;`，就必须写全：

```cpp
blink_led(1500ms);                    // ❌ 错误（未引入命名空间）
blink_led(1500ns);                    // ❌ 错误
blink_led(std::chrono::1500ms);       // ❌ 错误！不能这样写
blink_led(std::chrono::milliseconds(1500)); // ✅ 正确，但啰嗦
blink_led(1500 * std::chrono::milliseconds(1)); // ✅ 正确
```

所以最推荐：

```cpp
using namespace std::chrono_literals;
blink_led(1500ms);  // ✅ 简洁、清晰
```

------

## ✅ 二、函数参数类型定义

你的函数参数：

```cpp
void blink_led(std::chrono::milliseconds time_to_blink)
```

是合法的，但更通用的做法是使用 `auto` 或模板，接受任意时间单位：

### 方式 1：使用 `auto`（C++14 起）

```cpp
void blink_led(const std::chrono::milliseconds& time_to_blink)
```

### 方式 2：模板（最灵活）

```cpp
template <typename Rep, typename Period>
void blink_led(const std::chrono::duration<Rep, Period>& time_to_blink) {
    // 自动适配任何时间单位：ms, s, us, min 等
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_to_blink);
    // 使用 ms
}
```

这样你就可以传 `1500ms`、`1.5s`、`1500000us`，都会自动转换。

------

## ✅ 三、完整可运行示例

```cpp
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

void blink_led(std::chrono::milliseconds time_to_blink) {
    std::cout << "LED will blink for " 
              << time_to_blink.count() << " milliseconds\n";
}

int main() {
    blink_led(1500ms);   // 输出: LED will blink for 1500 milliseconds
    blink_led(2s);       // 输出: LED will blink for 2000 milliseconds
    blink_led(500us);    // 输出: LED will blink for 0 milliseconds（太小）
    return 0;
}
```

> ⚠️ 注意：`500us` 转成 `milliseconds` 会截断为 `0`，建议在函数内做 `duration_cast` 处理。

------

## ✅ 四、为什么这是好实践？

| 优点                    | 说明                                                         |
| ----------------------- | ------------------------------------------------------------ |
| ✅ **单位明确**          | `1500ms` 比 `1500` 清晰得多，避免“这是毫秒还是微秒？”的困惑  |
| ✅ **类型安全**          | 不能把 `seconds` 误传给期望 `milliseconds` 的函数（编译时报错） |
| ✅ **避免 magic number** | 消除“神秘数字”                                               |
| ✅ **自动换算**          | `std::chrono` 自动处理单位转换（需注意精度）                 |

------

## ✅ 五、其他可用的时间单位（`std::chrono`）

| 字面量 | 类型           | 含义 |
| ------ | -------------- | ---- |
| `1ns`  | `nanoseconds`  | 纳秒 |
| `1us`  | `microseconds` | 微秒 |
| `1ms`  | `milliseconds` | 毫秒 |
| `1s`   | `seconds`      | 秒   |
| `1min` | `minutes`      | 分钟 |
| `1h`   | `hours`        | 小时 |

------

## 🎯 总结

| 问题                             | 回答                                    |
| -------------------------------- | --------------------------------------- |
| C++ 函数可以输入带单位的参数吗？ | ✅ **可以！** 使用 `std::chrono`         |
| `1500ms` 这种写法合法吗？        | ✅ **合法**，C++14 起支持                |
| 需要什么头文件？                 | `#include <chrono>`                     |
| 需要 using 吗？                  | `using namespace std::chrono_literals;` |
| 是不是好设计？                   | ✅ **是！** 单位明确、类型安全、推荐实践 |

------

## 🎯 一句话记住：

> **“带上单位”是现代 C++ 的好习惯！**
> 用 `1500ms` 代替 `1500`，代码立刻变得更清晰、更安全。