## warning: range-based for loop is iterating over a temporary, which may result in unnecessary copies

```c++
for (auto it = map_face_ids.constBegin(); it != map_face_ids.constEnd(); ++it) {
        ui->face_id->addItem(QString::number(it.key()));
    }    
```

| 写法                                                  | 是否推荐 | 原因                     |
| ----------------------------------------------------- | -------- | ------------------------ |
| `for (auto face_id : map.keys())`                     | ❌ 不推荐 | 临时对象 + 拷贝警告      |
| `for (const auto& keys = map.keys(); face_id : keys)` | ✅ 推荐   | 明确保存临时值，避免警告 |
| `for (auto it = map.constBegin(); ... )`              | ✅ 推荐   | 遍历 map 更标准，零拷贝  |

## push_back 和   emplace_back

| 你要添加的是……     | 推荐用         | 理由               |
| ------------------ | -------------- | ------------------ |
| 已经构造好的对象   | `push_back`    | 不需要再构造一次   |
| 临时对象、构造参数 | `emplace_back` | 避免复制，提高效率 |

#  **C++ 里的成员函数指针**

```c++
NameWidget* (BoxStatus::*get_widget)(),
void (BoxStatus::*set_widget)(NameWidget*),
```

------

### 1. 普通函数指针

比如普通函数：

```cpp
int foo(int x);
```

它的函数指针写法是：

```cpp
int (*func_ptr)(int);
```

`func_ptr` 就是一个能指向 `foo` 的变量。

------

### 2. 成员函数指针

`BoxStatus` 里有成员函数：

```cpp
NameWidget* get_config_name_widget();
void set_config_name_widget(NameWidget*);
```

要指向这类“成员函数”，写法是：

```cpp
返回类型 (类名::*指针变量)(参数列表);
```

所以：

- `NameWidget* (BoxStatus::*get_widget)();`
   表示一个指针，指向 `BoxStatus` 的某个成员函数，这个成员函数 **没有参数**，返回 `NameWidget*`。
- `void (BoxStatus::*set_widget)(NameWidget*);`
   表示一个指针，指向 `BoxStatus` 的某个成员函数，这个成员函数 **参数是 `NameWidget\*`，返回 void**。

------

### 3. 怎么用？

假设你有：

```cpp
BoxStatus* box_status;
NameWidget* (BoxStatus::*getter)() = &BoxStatus::get_config_name_widget;
```

调用时要用 `->*` 或 `.*`：

```cpp
NameWidget* w = (box_status->*getter)();
```

同理，`set_widget`：

```cpp
void (BoxStatus::*setter)(NameWidget*) = &BoxStatus::set_config_name_widget;
(box_status->*setter)(w);
```

------

### 4. 在我给你的方案里

我传了 `get_widget` 和 `set_widget` 进去，这样 `start_save_process` 就能同时支持 “config” 和 “data”：

```cpp
if ((box_status->*get_widget)()) { ... }
(box_status->*set_widget)(widget);
```

这里 `(box_status->*get_widget)()` 就等于调用 `box_status->get_config_name_widget()` 或 `box_status->get_data_name_widget()`，具体取决于你传进去的是哪个指针。

------

✅ 总结：
 这段声明的意思就是：

- `get_widget` 是一个“指向 `BoxStatus` 成员函数（无参数，返回 `NameWidget*`）的指针”。
- `set_widget` 是一个“指向 `BoxStatus` 成员函数（参数是 `NameWidget*`，返回 void）的指针”。

这样就可以把“要操作的是 config 还是 data”抽象成参数传进来。

