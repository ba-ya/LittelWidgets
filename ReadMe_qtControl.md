[TOC]



# QToolButton

## 1.自带箭头,setArrowType

```c++
// 4种箭头样式切换
void MainWindow::on_btn_t_released()
{
    std::vector<Qt::ArrowType> list = {Qt::UpArrow, Qt::RightArrow, Qt::DownArrow, Qt::LeftArrow};
    ui->btn_t->setArrowType(list[flag]);
    flag = (flag + 1) % 4;
}
```

[参考1,博客]([Qt之QToolButton属性验证_qt arrowtype-CSDN博客](https://blog.csdn.net/weixin_40953784/article/details/104459823))

[参考2,官方]([QToolButton Class | Qt Widgets 6.8.3](https://doc.qt.io/qt-6/qtoolbutton.html))

## 2.弹出Action,setPopupMode

```c++
ui->btn_t->setPopupMode(QToolButton::DelayedPopup);
```

```c++
enum ToolButtonPopupMode {
        DelayedPopup,
        MenuButtonPopup,
        InstantPopup
    };
```

DelayedPopup,长按弹出,类似浏览器`<--`短按返回,长按显示历史记录

MenuButtonPopup,本身按钮+箭头按钮

InstantPopup,立即弹出,本身按钮的槽被覆盖,只剩下弹出功能

---

```c++
// 不要自带小箭头
ui->btn_t->setStyleSheet("QToolButton::menu-indicator { image: none; }");
```



