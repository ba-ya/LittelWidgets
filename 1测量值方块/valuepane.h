#ifndef VALUEPANE_H
#define VALUEPANE_H

#include <QWidget>

namespace Ui {
class ValuePane;
}
class QPushButton;
class DoubleSpinBoxWithUnit;

class ValuePane : public QWidget
{
    Q_OBJECT

public:
    explicit ValuePane(QWidget *parent = nullptr);
    ~ValuePane();

public:
    DoubleSpinBoxWithUnit *pane_value();
    void set_title(const QString &title);

private:
    void update_mode(int mode);

private:
    Ui::ValuePane *ui;
};

#endif // VALUEPANE_H
