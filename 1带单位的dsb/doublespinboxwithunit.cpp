#include "doublespinboxwithunit.h"

#include "ui_doublespinboxwithunit.h"

DoubleSpinBoxWithUnit::DoubleSpinBoxWithUnit(QWidget* parent) : QWidget(parent), ui(new Ui::DoubleSpinBoxWithUnit) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

DoubleSpinBoxWithUnit::~DoubleSpinBoxWithUnit() { delete ui; }

// softboard
void DoubleSpinBoxWithUnit::on_btn_unit_released() {
    ui->dsb_value->setFocus();
}

// dsb value
void DoubleSpinBoxWithUnit::on_dsb_value_valueChanged(double value) {
    emit valueChanged(value);
}

void DoubleSpinBoxWithUnit::on_dsb_value_editingFinished() {
    emit editingFinished();
}

double DoubleSpinBoxWithUnit::value() const {
    return ui->dsb_value->value();
}

void DoubleSpinBoxWithUnit::setValue(double value) {
    ui->dsb_value->setValue(value);
}

void DoubleSpinBoxWithUnit::setRange(double min, double max) {
    ui->dsb_value->setRange(min, max);
}

void DoubleSpinBoxWithUnit::setAlignment(Qt::Alignment flag) {
    ui->dsb_value->setAlignment(flag);
}

void DoubleSpinBoxWithUnit::set_focus_enabled(bool is_enable) {
    if (is_enable) {
        ui->dsb_value->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    } else {
        ui->dsb_value->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    }
    ui->btn_unit->setEnabled(is_enable);
}

// btn unit
void DoubleSpinBoxWithUnit::set_unit(const QString &unit)
{
    return ui->btn_unit->setText(unit);
}
