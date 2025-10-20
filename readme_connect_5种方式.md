---

### ✅ 五种连接方式总览

| 连接方式                                   | 枚举值                         | 适用场景       | 特点                                             |
| ------------------------------------------ | ------------------------------ | -------------- | ------------------------------------------------ |
| 自动连接（Auto Connection）                | `Qt::AutoConnection`           | 默认，推荐     | Qt 自动判断用“直接”还是“队列”                    |
| 直接连接（Direct Connection）              | `Qt::DirectConnection`         | 同一线程       | 槽函数**立即执行**，不经过事件循环               |
| 队列连接（Queued Connection）              | `Qt::QueuedConnection`         | 跨线程或同线程 | 槽函数**放入事件队列**，由事件循环异步执行       |
| 阻塞队列连接（Blocking Queued Connection） | `Qt::BlockingQueuedConnection` | 跨线程         | 槽函数放入事件队列，**发送线程阻塞等待执行完成** |
| 唯一连接（Unique Connection）              | `Qt::UniqueConnection`         | 任意           | 和其他方式组合使用，确保**不会重复连接**         |

---

### 📌 1. 自动连接 `Qt::AutoConnection`（默认）

```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot);
// 等价于：
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::AutoConnection);
```

- **行为**：Qt 自动判断：
  - 如果 `sender` 和 `receiver` 在**同一个线程** → 使用 **直接连接**
  - 如果在**不同线程** → 使用 **队列连接**
- **优点**：省心，推荐大多数情况使用。
- **底层实现**：Qt 在 `connect()` 时检查两个对象的线程（`thread()`），动态决定连接类型。

---

### 📌 2. 直接连接 `Qt::DirectConnection`

```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::DirectConnection);
```

- **行为**：
  - 信号一发出，**立刻调用槽函数**，就像普通函数调用。
  - **不经过事件循环**。
  - 槽函数在**发送信号的线程**中执行。
- **适用**：同一线程内，追求高性能、低延迟。
- **风险**：如果槽函数很耗时，会阻塞信号发送线程。

> 🔧 类比：打电话直接接通，对方必须马上接。

---

### 📌 3. 队列连接 `Qt::QueuedConnection`

```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::QueuedConnection);
```

- **行为**：
  - 信号发出后，**不会立即执行槽**。
  - Qt 将槽函数调用“打包”成一个事件，放入 `receiver` 所在线程的**事件队列**。
  - 由该线程的**事件循环**（`QEventLoop`）在适当时候取出并执行。
- **适用**：跨线程通信（最常见场景）。
- **特点**：
  - 发送线程**不会阻塞**。
  - 槽函数在 `receiver` 的线程中执行。
  - 参数必须是**可被 Qt 元对象系统识别的类型**（用 `Q_DECLARE_METATYPE` 注册）。

> 🔧 类比：发邮件，对方稍后查看并处理。

---

### 📌 4. 阻塞队列连接 `Qt::BlockingQueuedConnection`

```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::BlockingQueuedConnection);
```

- **行为**：
  - 类似于 `QueuedConnection`，但**发送线程会阻塞**，直到槽函数执行完毕。
  - 槽函数仍在 `receiver` 所在线程中执行。
- **适用**：跨线程，且需要等待结果（类似同步调用）。
- **注意**：
  - `receiver` 所在线程**必须有正在运行的事件循环**，否则会死锁！
  - 不能在 GUI 线程（主线程）中使用，否则界面会卡死。

> ⚠️ 风险：容易死锁，慎用！

> 🔧 类比：发信后站在门口等对方回信，不回就不走。

---

### 📌 5. 唯一连接 `Qt::UniqueConnection`

```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::UniqueConnection);
```

- **行为**：
  - 这不是一个独立的连接类型，而是**和其他类型组合使用**。
  - 它的作用是：**如果这个信号和槽已经连接过，就不再重复连接**，`connect()` 返回 `false`。
- **适用**：防止多次连接导致槽函数被调用多次。
- **通常写法**：
  ```cpp
  bool connected = connect(sender, &Sender::signal, receiver, &Receiver::slot,
                           static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection));
  if (!connected) {
      qDebug() << "连接已存在或失败";
  }
  ```

> 🔧 类比：订阅杂志，如果已经订了，就不允许再订一次。

---

### 💡 代码示例：跨线程使用队列连接

```cpp
// worker.h
class Worker : public QObject {
    Q_OBJECT
public slots:
    void doWork() {
        qDebug() << "工作线程执行任务：" << QThread::currentThread();
    }
};

// main.cpp
QThread *thread = new QThread;
Worker *worker = new Worker;
worker->moveToThread(thread);

// 使用队列连接，确保槽在子线程执行
connect(this, &MainWindow::startWork, worker, &Worker::doWork, Qt::QueuedConnection);

thread->start();
emit startWork(); // 触发
```

---

### ✅ 总结一句话

| 连接方式 | 关键行为                | 代码体现                           |
| -------- | ----------------------- | ---------------------------------- |
| 自动连接 | Qt 自动选“直接”或“队列” | `Qt::AutoConnection`（默认）       |
| 直接连接 | 立刻执行，同线程        | `Qt::DirectConnection`             |
| 队列连接 | 异步执行，跨线程安全    | `Qt::QueuedConnection`             |
| 阻塞队列 | 同步等待，跨线程        | `Qt::BlockingQueuedConnection`     |
| 唯一连接 | 防止重复连接            | `Qt::UniqueConnection`（组合使用） |

> 🎯 **推荐**：大多数情况用**默认自动连接**，跨线程明确用 `QueuedConnection`，防重复用 `UniqueConnection`。

理解这五种方式，你就能精准控制信号槽的执行时机和线程安全了！