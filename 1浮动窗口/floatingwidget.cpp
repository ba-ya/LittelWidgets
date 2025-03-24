#include "FloatingWidget.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>

FloatingWidget::FloatingWidget(QWidget *parent) : QWidget(parent) {
    // setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
}

// 需要给出layout和行列值
// eg. dynamic_cast<FloatingWidget*>(widget_ascan[i][0])->setPosition(layout_abscan[i], i, 0, 0);
void FloatingWidget::setPosition(QGridLayout* layout, int status_id, int row, int col) {
    switch (status_id) {
    // setting
    case 10:
        setWindowTitle(col == 0 ? QString("Ascan_%1").arg(row + 1) : QString("Bscan_%1").arg(row + 1));
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        setWindowTitle(QString("%1: Bscan_%2").arg(status_id - 10).arg(row + 1));
        break;
    // inspect, analysis
    case 20:
        setWindowTitle(QString("group_%1").arg(row * 2 + col + 1));
        break;
    // inspect single
    case 30:
        setWindowTitle(QString("bar_%1").arg(row + 1));
        break;
    }

    originalLayout = layout;
    originalRow = row;
    originalColumn = col;
}

void FloatingWidget::toggleMaximize() {
    if (!isMaximized) {
        setWindowFlags(Qt::Window);
        setWindowModality(Qt::ApplicationModal);

        QRect screenGeometry = QApplication::desktop()->screenGeometry();
        int x = (screenGeometry.width() - width()) / 2 + screenGeometry.x();
        int y = (screenGeometry.height() - height()) / 2 + screenGeometry.y();
        move(x, y);

        show();
        isMaximized = true;
    } else {
        setWindowFlags(Qt::Widget);
        setWindowModality(Qt::NonModal);
        originalLayout->addWidget(this, originalRow, originalColumn);
        isMaximized = false;
    }
}

// event
void FloatingWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    // qDebug() << "mouseDoubleClickEvent";
    toggleMaximize();
    QWidget::mouseDoubleClickEvent(event);
}

void FloatingWidget::closeEvent(QCloseEvent *event) {
    // qDebug() << "closeEvent";
    toggleMaximize();
    event->accept();
}
