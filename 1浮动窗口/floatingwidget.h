#ifndef FLOATINGWIDGET_H
#define FLOATINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHoverEvent>
#include <QDialog>

class FloatingWidget : public QWidget {
    Q_OBJECT

public:
    explicit FloatingWidget(QWidget *parent = nullptr);

public:
    void setPosition(QGridLayout* layout, int status_id, int row, int col);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void toggleMaximize();

private:
    QGridLayout* originalLayout;
    int originalRow = 0;
    int originalColumn = 0;
    bool isMaximized = false;
};

#endif // FLOATINGWIDGET_H
