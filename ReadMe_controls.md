
- [绑定信号QOverload](#绑定信号qoverload)
- [QToolButton](#qtoolbutton)
  - [1.自带箭头,setArrowType](#1自带箭头setarrowtype)
  - [2.弹出Action,setPopupMode](#2弹出actionsetpopupmode)
- [QDialog](#qdialog)
  - [1.显示最大化,关闭按钮;有阻塞动画](#1显示最大化关闭按钮有阻塞动画)
- [QMessageBox](#qmessagebox)
  - [timeout后自动消失](#timeout后自动消失)
- [QCombobox](#qcombobox)
  - [下拉列表顺序排列](#下拉列表顺序排列)
- [QTablewidget](#qtablewidget)
  - [添加复选框,勾选选中整行](#添加复选框勾选选中整行)
    - [`QTableWidgetSelectionRange(topRow, leftColumn, bottomRow, rightColumn)`](#qtablewidgetselectionrangetoprow-leftcolumn-bottomrow-rightcolumn)
  - [初始化](#初始化)
  - [填充行](#填充行)
  - [`setCellWidget()`](#setcellwidget)
  - [滚动到最后一行](#滚动到最后一行)

# 绑定信号QOverload

```c++
QOverload<int>::of(&QSpinBox::valueChanged),
```

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

# QDialog

## 1.显示最大化,关闭按钮;有阻塞动画

点击其他窗口,阻塞窗口会闪烁

```
auto dlg = new SettingPage(this);  // 设置父窗口
dlg->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint);
dlg->exec();  // 阻塞，模态效果完整
```

# QMessageBox

```c++
// 阻塞, 静态
QMessageBox::information(this, "Title", "Content");
```

```c++
// 阻塞, 自定义
QMessageBox msg(this);
msg.setWindowTitle("Title");           // 设置标题
msg.setText("Content");               // 设置主文本
msg.setIcon(QMessageBox::Information);                   // 设置图标为“信息”图标
msg.setStandardButtons(QMessageBox::Ok);                 // 设置按钮为“OK”
msg.exec();                                              // 以模态方式运行
```

## timeout后自动消失 

```c++
void show_toast(QWidget *parent, const QString &title, const QString &content, int timeout_ms)
{
    QMessageBox *msg = new QMessageBox(parent);
    msg->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动 delete
    msg->setWindowTitle(title);
    msg->setText(content);
    msg->setIcon(QMessageBox::Information);
    msg->setStandardButtons(QMessageBox::NoButton);
    msg->show();
    QTimer::singleShot(timeout_ms, msg, [msg]() {
        msg->done(QMessageBox::Ok);
    });
}
```

# QCombobox

## 下拉列表顺序排列

[link]([Qt中QComboBox下拉列表（popup）位置与样式的控制_qt popup-CSDN博客](https://blog.csdn.net/imred/article/details/78158238))

```css
QComboBox{
    combobox-popup:0;
}
```

# QTablewidget

## 添加复选框,勾选选中整行

```c++
    // 准备table状态,不可选中
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);
```

```c++
        //添加复选框
		auto item_name = new QTableWidgetItem(dir);
        item_name->setFlags(item_name->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item_name->setCheckState(Qt::Unchecked);
```

```c++
	// 点击复习框所在的单元格,选中整行
    connect(ui->table, &QTableWidget::itemClicked, this, [this](QTableWidgetItem *item) {
        if (item->column() != 0 || !(item->flags() & Qt::ItemIsUserCheckable)) {
            return;
        }
        update_row_range_check(item->row(), item->row(), !(item->checkState() == Qt::Checked));
    });
	// 勾选复选框,选中整行
    connect(ui->table, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *item) {
        if (item->column() != 0 || !(item->flags() & Qt::ItemIsUserCheckable)) {
            return;
        }
        update_row_range_check(item->row(), item->row(), item->checkState() == Qt::Checked);
    });

```

```c++
// 选中或不选中, [row_start, row_end]的所有行
// 同时同步复选框状态
void update_row_range_check(int row_start, int row_end, bool checked)
{
    ui->table->blockSignals(true);
    for (int i = row_start; i <= row_end; ++i) {
        ui->table->item(i, 0)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }
    ui->table->setRangeSelected(
        QTableWidgetSelectionRange(row_start, 0, row_end, ui->table->columnCount() - 1),
        checked
        );
    ui->table->blockSignals(false);
}
```

### `QTableWidgetSelectionRange(topRow, leftColumn, bottomRow, rightColumn)`

这是一个类，用来表示表格中的一个矩形区域：

| 参数          | 含义                      |
| ------------- | ------------------------- |
| `topRow`      | 区域的起始行（从 0 开始） |
| `leftColumn`  | 区域的起始列（从 0 开始） |
| `bottomRow`   | 区域的结束行              |
| `rightColumn` | 区域的结束列              |

这个四个参数定义了一个**矩形区域**：从 `(topRow, leftColumn)` 到 `(bottomRow, rightColumn)`。

## 初始化

初始化

横向表头拉伸

表格item不可编辑

失去焦点仍有颜色

```c++
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QPalette p = ui->table->palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    ui->table->setPalette(p);
    connect(ui->table, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item){
        // do something
    });
```

## 填充行

```
ui->table->setRowCount(0);

    int cnt_row = parts.size() / 2;
    ui->table->setRowCount(cnt_row);

    for (int i = 0; i < cnt_row; ++i) {
        int beam = parts.at(i * 2).toInt();
        int point = parts.at(i * 2 + 1).toInt();

        ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(beam)));
        ui->table->setItem(i, 1, new QTableWidgetItem(QString::number(point)));
    }
```

## `setCellWidget()` 

当你使用 `setCellWidget()` 放入任意控件（如 `QCheckBox`、`QLineEdit`、`QPushButton` 等），这个控件就**脱离了 `QTableWidgetItem` 的控制范围**，**它的行为和信号不会触发 `QTableWidget` 的原生信号**，你必须自己手动连接和管理它们。

## 滚动到最后一行

```c++
ui->table->scrollToBottom();
```

