# 保留两位小数

[参考](https://blog.csdn.net/weixin_40314351/article/details/127568393)

```c++
 static double keep_decimal_places(double value, int decimal = 2) {
        value = QString::number(value, 'f', decimal).toDouble();
        return value;
    }
```

