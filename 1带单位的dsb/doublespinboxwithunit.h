#ifndef DOUBLESPINBOXWITHUNIT_H
#define DOUBLESPINBOXWITHUNIT_H

#include <QWidget>

namespace Ui {
class DoubleSpinBoxWithUnit;
}
class QDoubleSpinBox;
class DoubleSpinBoxWithUnit : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleSpinBoxWithUnit(QWidget *parent = nullptr);
    ~DoubleSpinBoxWithUnit();

public:
    // value
    double value() const;
    void setValue(double);
    void setRange(double min, double max);
    void setAlignment(Qt::Alignment flag);
    void set_focus_enabled(bool is_enable);
    // unit
    void set_unit(const QString &unit);

signals:
    void valueChanged(double value);
    void editingFinished();

private slots:
    void on_btn_unit_released();
    void on_dsb_value_valueChanged(double arg1);

    void on_dsb_value_editingFinished();

private:
    Ui::DoubleSpinBoxWithUnit *ui;
};

#endif // DOUBLESPINBOXWITHUNIT_H
