- [*Qt常用代码*](#qt常用代码)
  - [添加图标](#添加图标)
  - [获取当前时间](#获取当前时间)
  - [Widget关闭时发送destroyed信号](#widget关闭时发送destroyed信号)
  - [浏览器打开目录或文件](#浏览器打开目录或文件)
  - [正则](#正则)
  - [chartview](#chartview)
  - [editingFinished含义](#editingfinished含义)
  - [~~lambda绑定信号,QOverload~~](#lambda绑定信号qoverload)
  - [Combobox居中显示](#combobox居中显示)
- [*Common*](#common)
  - [文件操作](#文件操作)
  - [移动到新线程](#移动到新线程)
  - [截图](#截图)
  - [qt控件](#qt控件)
  - [VTK\_MODULE\_INIT对应头文件](#vtk_module_init对应头文件)
  - [vtk交互样式](#vtk交互样式)
  - [按钮点击弹出窗口](#按钮点击弹出窗口)
  - [dump文件](#dump文件)
  - [日志](#日志)
  - [单例切换语言](#单例切换语言)
  - [单例切换样式](#单例切换样式)
- [*CommonWidget*](#commonwidget)
  - [带act的lineEdit](#带act的lineedit)
    - [显示](#显示)
  - [tablewidget](#tablewidget)
    - [tablewidget填充行](#tablewidget填充行)
    - [显示](#显示-1)
  - [浮动窗口](#浮动窗口)
    - [显示](#显示-2)
  - [软键盘](#软键盘)
    - [显示](#显示-3)
  - [带单位的dsb](#带单位的dsb)
    - [显示](#显示-4)
  - [测量值方块](#测量值方块)
    - [显示](#显示-5)
  - [tcg窗口](#tcg窗口)
    - [显示](#显示-6)


# *Qt常用代码*

## 添加图标

pro文件

```c++
RC_ICONS = xxx.ico
```

cmake

```
// icon.rc内容
IDI_ICON1 ICON DISCARDABLE "img.ico"
```

```
qt_add_executable(...
	...
	icon.rc
)
```

## 获取当前时间

```
QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")
```

## Widget关闭时发送destroyed信号

```c++
this->setAttribute(Qt::WA_DeleteOnClose);
connect(this, &QObject::destroyed, this, [this]() {
    //...
});
```

## 浏览器打开目录或文件

```c++
    QFileInfo fileInfo(save_path);
    QString absolute_path = fileInfo.absoluteFilePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(absolute_path));
```

## 正则

```c++
 QRegularExpression regex(R"(namespace\s*(\w+)\s*\{)");
            QRegularExpressionMatch match = regex.match(content);
            if (match.hasMatch()) {
                item_str += QString("(%1)").arg(match.captured(1));
            }
```



## chartview

tooltip

```c++
        m_series = new QScatterSeries();
        m_chart->addSeries(m_series);
        m_series->attachAxis(xAxis);
        m_series->attachAxis(yAxis);
        connect(m_series, &QScatterSeries::hovered, this, &ChartViewSC::showTooltip);

        tooltip = new QGraphicsSimpleTextItem(m_chart);
        tooltip->hide();
```

```c++
void showTooltip(const QPointF &point, bool state) {
        if (state) {
            tooltip->setText(QString("(%1, %2%)")
                                 .arg(point.x())
                                 .arg(QString::number(point.y() / y_end * 100, 'f', 2)));
            QPointF position = m_chart->mapToPosition(point, m_series);
            tooltip->setPos(position.x() - 40, position.y() - 40);

            tooltip->show();
        } else {
            tooltip->hide();
        }
    }
```

## editingFinished含义

`QLineEdit`中`void editingFinished()`表示 在 `回车` 或者 `返回` 或者 `行编辑失去焦点`的情况发送信号

## ~~lambda绑定信号,QOverload~~

```c++
QOverload<int>::of(&QComboBox::currentIndexChanged) // Qt6弃用QString参数
```

```c++
QOverload<int>::of(&QSpinBox::valueChanged) // Qt6弃用QString参数
```

## Combobox居中显示

```c++
	auto model = static_cast<QStandardItemModel*>(ui->frequence->model());
    for (int i = 0; i < ui->frequence->count(); i++) {
        model->item(i)->setTextAlignment(Qt::AlignCenter);
    }
```

# *Common*

## [文件操作](./ReadMe_file.md)

## 移动到新线程

```cpp
    // mainwindow.cpp
    data_proc = new DataProcTfm();
    thread_data = new QThread();
    data_proc->moveToThread(thread_data);
    connect(thread_data, &QThread::started, data_proc, &DataProcTfm::thread_start);
    connect(thread_data, &QThread::finished, data_proc, &DataProcTfm::thread_stop);

    qDebug() << "this:" << QThread::currentThreadId();
    thread_data->start();
```

```cpp
void DataProcTfm::thread_start()
{
    qDebug() << "thread_start:"<< QThread::currentThreadId();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &DataProcTfm::on_timer);

    connect(this, &DataProcTfm::start_data_flow, this, [this]() {
        if (timer->isActive()) {
            return;
        }
        timer->start(interval_t);
        qDebug() << "start_data_flow";
    });

    connect(this, &DataProcTfm::stop_data_flow, this, [this]() {
        timer->stop();
        qDebug() << "stop_data_flow";
    });

    connect(this, &DataProcTfm::set_prop, this, [this](int x, int y, int z) {
        cnt_x = x;
        cnt_y = y;
        cnt_z = z;
        qDebug() << "set_prop, cnt:" << cnt_x << "," << cnt_y << "," << cnt_z;
    });
}
void DataProcTfm::thread_stop()
{
    timer->stop();
}
```



## 截图

QWidget自带截图

```c++
QPixmap pixmap = this->centralWidget()->grab();
pixmap.save(file);
```

## [qt控件](./ReadMe_controls.md)

## VTK_MODULE_INIT对应头文件

```c++
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
```

## vtk交互样式

vtkStandardNewMacro一定要有, 对应头文件vtkObjectFactory.h

```c++
// h
#ifndef INSPECT_VIEW_INTERACTOR_STYLE_H
#define INSPECT_VIEW_INTERACTOR_STYLE_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

class OnlyShowTipInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static OnlyShowTipInteractorStyle* New();
    vtkTypeMacro(OnlyShowTipInteractorStyle, vtkInteractorStyleTrackballCamera);

    void OnMouseMove() override;
};

#endif // INSPECT_VIEW_INTERACTOR_STYLE_H

```

```c++
// cpp
#include "inspect_view_interactor_style.h"

vtkStandardNewMacro(OnlyShowTipInteractorStyle);

void OnlyShowTipInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

```

## 按钮点击弹出窗口

```c++
struct BoxStatus
{
    void set_ascan_widget(AscanWidgetView *w) { this->ascan_widget = w; }
    AscanWidgetView *get_ascan_widget() { return ascan_widget; }

private:
    AscanWidgetView *ascan_widget;
};
```

```c++
 box_status = std::make_shared<BoxStatus>();
```

```c++
void Inspect::on_pop_ascan_released()
{
    if (box_status->get_ascan_widget()) {
        box_status->get_ascan_widget()->showNormal();
        box_status->get_ascan_widget()->raise();
        box_status->get_ascan_widget()->activateWindow();
        return;
    }
    auto widget = new AscanWidgetView();
    box_status->set_ascan_widget(widget);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    connect(widget, &QObject::destroyed, this, [this]() { box_status->set_ascan_widget(nullptr); });
    widget->show();
}
```

```c++
// 退出主窗口销毁弹出窗口
if (box_status->get_ascan_widget()) {
        delete box_status->get_ascan_widget(); 
        box_status->set_ascan_widget(nullptr); 
}
```

## [dump文件](./0code/dump.md)

## [日志](./0日志)

main里面调用,

可以配合自动生成的配置文件一起使用

```c++
aux::Logger::instance().setLogPrefix("phaselink_server");// 自定义前缀
// Init Logger
aux::Logger::instance().EnableConsole();
aux::Logger::instance().EnableFile(aux::ConfigOutput::instance().dump_log_type);
qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    aux::Logger::instance().Log(type, context, msg);
});
```

![Snipaste_2025-01-09_11-27-50](./ReadMe.assets/Snipaste_2025-01-09_11-27-50.png)

## [单例切换语言](./0单例切换语言)

```c++
// 触发变化
LanguageManager::instance().switchLanguage(index);

// 其他类接受变化
connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, &SettingPage::update_lang);
```

## [单例切换样式](./0单例切换样式)

```c++
// 触发变化
StyleManager::instance().switchStyle(mode);

// 其他类接受变化
connect(&StyleManager::instance(), &StyleManager::styleChanged, this, &ValuePane::update_mode);

//根据pm,am决定mode,并提取qss中对应行,eg
void ValuePane::update_mode(int mode) {
    auto style = StyleManager::instance().styleString(":/dsb_with_title.qss", mode);
    ui->pane_value->setStyleSheet(style);
}
```

[基础qss](./0单例切换样式/style.qss),其他窗口有什么特殊变化,可以在收到信号后,再set自己的stylesheet

# *CommonWidget*

## [带act的lineEdit](./1带act的lineEdit/image)

```c++
#include <QFileDialog>
```

```c++
    auto act = new QAction(this);
    act->setIcon(QIcon(":/image/folder.svg"));
    ui->file->addAction(act, QLineEdit::TrailingPosition);
    connect(act, &QAction::triggered, this, &MainWindow::chose_file);
    connect(act, &QAction::triggered, this, &MainWindow::chose_dir);
```

```c++
void MainWindow::chose_file()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr(u8"选择文件"), QDir::currentPath(), tr(u8"*(*.*)"));
    if (filePath.isEmpty()) {
        return;
    }
    ui->file->setText(filePath);
    
    // do something...
}

void MainWindow::chose_dir()
{
     QString loadDir = QFileDialog::getExistingDirectory(this, tr(u8"选择目录"), QDir::currentPath());
    if (loadDir.isEmpty()) {
        return;
    }
    ui->file->setText(loadDir);
}
```

### 显示

![image-20250113150558280](./ReadMe.assets/image-20250113150558280.png)

## tablewidget

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

### tablewidget填充行

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

当你使用 `setCellWidget()` 放入任意控件（如 `QCheckBox`、`QLineEdit`、`QPushButton` 等），这个控件就**脱离了 `QTableWidgetItem` 的控制范围**，**它的行为和信号不会触发 `QTableWidget` 的原生信号**，你必须自己手动连接和管理它们。

### 显示

![image-20250113154915412](./ReadMe.assets/image-20250113154915412.png)

## [浮动窗口](./1浮动窗口)

```c++
// 在使用之前,要先把窗口在栅格布局的中行列给它,eg.
dynamic_cast<FloatingWidget*>(widget_ascan[i][0])->setPosition(layout_abscan[i], i, 0, 0);
```

### 显示

![Snipaste_2024-10-16_15-29-22](./ReadMe.assets/Snipaste_2024-10-16_15-29-22.png)

## [软键盘](./1软键盘)

需要和[单例切换样式中的qss](./0单例切换样式/style.qss)配套使用,查找'softboard'

### 显示

![image-20250109115313478](./ReadMe.assets/image-20250109115313478.png)

![image-20250109115319043](./ReadMe.assets/image-20250109115319043.png)

## [带单位的dsb](./1带单位的dsb)

需要和[单例切换样式中的qss](./0单例切换样式/style.qss)配套使用,查找'DoubleSpinBoxwithUnit'

### 显示

![image-20250109121504418](./ReadMe.assets/image-20250109121504418.png)

## [测量值方块](./1测量值方块)

需要和[单例切换样式中的qss](./0单例切换样式/style.qss)配套使用,查找'ValuePane'

并且因为结合'带单位的dsb'做出来的,'带单位的dsb'样式要修改成[这种](./1测量值方块/dsb_with_title.qss)

### 显示

![image-20250109121459364](./ReadMe.assets/image-20250109121459364.png)

## [tcg窗口](./1tcg)

### 显示

![Snipaste_2024-10-16_15-29-53](./ReadMe.assets/Snipaste_2024-10-16_15-29-53.png)

