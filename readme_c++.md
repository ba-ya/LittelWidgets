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