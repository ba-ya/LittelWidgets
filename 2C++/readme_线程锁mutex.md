*资源获取即初始化（Resource Acquisition Is Initialization）*，简称 RAII

```c++
    std::mutex mtx;
    std::lock_guard<std::mutex> guard(mtx);
```

```c++
QMutex mutex; 
QMutexLocker locker(&mutex);
```