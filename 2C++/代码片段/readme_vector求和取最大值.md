[TOC]

# 求最大值

```c++
    auto it = max_element(nums.begin(), nums.end());
     int tatget = *it;
```

| 函数                            | 作用                                                   | 参数类型                         |
| ------------------------------- | ------------------------------------------------------ | -------------------------------- |
| `std::max(a, b)`                | 比较两个值，返回较大的那个                             | 两个具体值（如 `int`, `double`） |
| `std::max_element(first, last)` | 在迭代器范围内找最大元素，返回**指向最大元素的迭代器** | 迭代器范围 `[begin, end)`        |

# 求和

## // accumulate

```c++
int sum =  std::accumulate(vec.begin(), vec.end(), 0);
```

```
#include <numeric>  // 注意：不是 <algorithm>
T sum = std::accumulate(first, last, init);
```

- `first` : 起始迭代器（如 `vec.begin()`）
- `last` : 结束迭代器（如 `vec.end()`）
- `init` : 初始值（累加从这个值开始）

你提到了 `reduce(nums.begin(), nums.end())`，这是一个非常现代且重要的问题！

我们来详细解析：`std::reduce` 和 `std::accumulate` 的区别、用法和适用场景。

## // reduce

```c++
int sum1 = std::reduce(nums.begin(), nums.end(), 0);
```

```cpp
#include <numeric>
T result = std::reduce(exec_policy, first, last, init, binary_op);
```

- `exec_policy`（可选）：执行策略，如 `std::execution::par`（并行）
- `first`, `last`：迭代器范围
- `init`：初始值
- `binary_op`（可选）：二元操作（如 `+`, `*`, `std::plus<>()`）

# 取子区间[i, j]

---

## ✅ 正确方法：使用构造函数 + 迭代器

```cpp
std::vector<int> sub(nums.begin() + i, nums.begin() + j + 1);
```

### 🔍 参数说明：
- `nums.begin() + i`：指向第 `i` 个元素的迭代器（起始位置，包含）
- `nums.begin() + j + 1`：指向第 `j+1` 个元素的迭代器（结束位置，**不包含**）

> 📌 STL 的区间是 **左闭右开** `[start, end)`，所以要加 `+1` 才能包含 `j`。

---

### ✅ 示例代码

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> nums = {10, 20, 30, 40, 50, 60};
    int i = 1, j = 3;

    // 提取 [i, j] 区间，即 [1,3] -> {20,30,40}
    std::vector<int> sub(nums.begin() + i, nums.begin() + j + 1);

    for (int x : sub) {
        std::cout << x << " ";  // 输出: 20 30 40
    }

    return 0;
}
```

---

## ⚠️ 边界检查（推荐）

在实际使用中，建议先检查索引是否合法：

```cpp
if (i >= 0 && j < nums.size() && i <= j) {
    std::vector<int> sub(nums.begin() + i, nums.begin() + j + 1);
} else {
    // 处理越界情况
}
```
