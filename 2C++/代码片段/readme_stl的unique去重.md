```c++
  std::vector<double> dims;
  // ...
  std::sort(dims.begin(), dims.end());
  dims.erase(
      std::unique(dims.begin(), dims.end(), [](double a, double b){ return std::abs(a - b) < 1e-6; }),
      dims.end()
  );
```

------

### 🌱 背景：`std::unique` 并不真的“删除”元素

`std::unique` 是 `<algorithm>` 里的函数，它的作用是：

> 把 **连续的、相邻的相同元素** 移到容器前面，并返回“去重后范围的末尾迭代器”。

也就是说：

- 它不会真正删除元素；
- 它只是**把不重复的元素挪到前面**；
- 然后返回一个“新的逻辑结尾”（迭代器）；
- 如果你想真正删除后面的重复部分，就要再配合 `.erase()`。

------

### 🍀 这行代码拆解讲解

```cpp
dims.erase(
    std::unique(dims.begin(), dims.end(),
        [](double a, double b){ return std::abs(a - b) < 1e-6; }
    ),
    dims.end()
);
```

我们从内往外看：

#### 1️⃣ `std::unique(dims.begin(), dims.end(), predicate)`

- 它扫描整个 `dims` 向量；
- 每次比较相邻两个元素 `a` 和 `b`；
- 如果 `predicate(a, b)` 返回 `true`，说明它们“被认为相等”，`unique` 会**保留前一个，丢弃后一个**；
- 返回一个迭代器，指向**去重后有效范围的末尾**。

> ⚠️ 注意：它只比较**相邻元素**，所以在调用前一般要 `std::sort(dims.begin(), dims.end());`。

------

#### 2️⃣ `[](double a, double b){ return std::abs(a - b) < 1e-6; }`

这是一个 **Lambda表达式**（匿名函数），定义“两个值是否相等”的规则。

它的逻辑：

- 如果两个浮点数的差的绝对值小于 `1e-6`，就认为它们相等；
- 这是因为浮点运算存在误差（比如 0.1 + 0.2 ≠ 0.3）。

所以这行是**带容差的相等判断**。

------

#### 3️⃣ `.erase(new_end, dims.end())`

因为 `unique` 只是“移动”元素，并没真正删除，
 所以我们用 `.erase()` 删除逻辑上“无效”的尾部。

例如：

```
原始 dims: [2.000001, 2.0, 5.0, 5.0, 10.0]
std::unique 之后: [2.000001, 5.0, 10.0, 5.0, 10.0]
unique 返回指向 10.0（第3个元素后面）
.erase(...) 之后: [2.000001, 5.0, 10.0]
```

------

### 🧭 总结一句话

> 这行代码是 C++ 标准的“去重模板”，
>  它在有浮点误差的情况下，删除相邻、几乎相等的元素。

完整逻辑可以理解为：

```cpp
// 先排序（保证相邻元素可比较）
std::sort(dims.begin(), dims.end());

// 去除重复值（带容差）
auto new_end = std::unique(dims.begin(), dims.end(),
    [](double a, double b){ return std::abs(a - b) < 1e-6; });

// 删除多余尾部元素
dims.erase(new_end, dims.end());
```

