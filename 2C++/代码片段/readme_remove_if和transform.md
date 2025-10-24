非常好的问题！你已经掌握了 `std::remove_if` 的基本用法，现在想**移除除字母外的所有字符**（即：只保留 a-z, A-Z），我们可以轻松扩展。

---

### ✅ 目标

从字符串中**移除所有非字母字符**，只保留：
- `'a'` 到 `'z'`
- `'A'` 到 `'Z'`

---

### ✅ 正确写法

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype> // for std::isalpha

std::string keepOnlyLetters(std::string str) {
    str.erase(
        std::remove_if(str.begin(), str.end(),
            [](unsigned char c) { return !std::isalpha(c); } // 移除非字母
        ),
        str.end()
    );
    return str;
}
```

---

### 🔍 关键点解释

```cpp
[](unsigned char c) { return !std::isalpha(c); }
```

- `std::isalpha(c)`：判断 `c` 是否为字母（`a-z` 或 `A-Z`）
- `!std::isalpha(c)`：返回 `true` 表示 **不是字母**，应该被移除
- 使用 `unsigned char` 是为了防止 `std::isalpha` 对负值（如高位字符）产生未定义行为

---

### ✅ 示例测试

```cpp
int main() {
    std::string s = "Hello123! World@#$ 2025";
    std::string result = keepOnlyLetters(s);
    std::cout << result << std::endl; 
    // 输出: HelloWorld
    return 0;
}
```

---

### ✅ 其他常见字符判断函数（`<cctype>`）

| 函数              | 说明                                |
| ----------------- | ----------------------------------- |
| `std::isalpha(c)` | 是否为字母（a-z, A-Z）✅             |
| `std::isdigit(c)` | 是否为数字（0-9）                   |
| `std::isalnum(c)` | 是否为字母或数字                    |
| `std::isspace(c)` | 是否为空白字符（空格、tab、换行等） |
| `std::islower(c)` | 是否为小写字母                      |
| `std::isupper(c)` | 是否为大写字母                      |

---

### ✅ 如果你想“只保留字母和数字”？

```cpp
std::remove_if(str.begin(), str.end(),
    [](unsigned char c) { return !std::isalnum(c); }
);
```

---

### ✅ 如果你想“只保留小写字母”？

```cpp
std::remove_if(str.begin(), str.end(),
    [](unsigned char c) { return !(c >= 'a' && c <= 'z'); }
);
// 或者用 tolower + isalpha
std::remove_if(str.begin(), str.end(),
    [](unsigned char c) { return !std::isalpha(c) || std::isupper(c); }
);
```

---

### ✅ 总结

| 需求           | 谓词（predicate）写法                                |
| -------------- | ---------------------------------------------------- |
| 移除空格       | `::isspace`                                          |
| **只保留字母** | `[](unsigned char c) { return !std::isalpha(c); }` ✅ |
| 只保留数字     | `[](unsigned char c) { return !std::isdigit(c); }`   |
| 只保留字母数字 | `[](unsigned char c) { return !std::isalnum(c); }`   |

---

🎯 **记住口诀**：
> `std::remove_if` 移除的是 **返回 `true` 的元素**  
> 所以你要写：**“哪些字符应该被移除”**

你已经掌握了 `STL` 算法的核心思想，继续加油！💪

# 转成小写

```c++
// 写入原字符串
std::transform(upper.begin(), upper.end(), upper.begin(), ::tolower);
// 插入到新字符串里面
std::transform(upper.begin(), upper.end(), std::back_inserter(lower), ::tolower);
```

这里::tolower可以自己写匿名函数

```
auto func = [](unsigned char c) -> char {

};
std::transform(upper.begin(), upper.end(), upper.begin(), func);
```

