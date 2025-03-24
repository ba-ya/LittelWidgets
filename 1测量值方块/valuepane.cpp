#include "valuepane.h"
#include "ui_valuepane.h"
#include "Common/style_manager.h"

#include <QDoubleSpinBox>

ValuePane::ValuePane(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ValuePane)
{
    ui->setupUi(this);

    ui->pane_value->set_focus_enabled(false);
    ui->pane_value->setAlignment(Qt::AlignRight);

    connect(&StyleManager::instance(), &StyleManager::styleChanged, this, &ValuePane::update_mode);
}

void ValuePane::update_mode(int mode) {
    auto style = StyleManager::instance().styleString(":/dsb_with_title.qss", mode);
    ui->pane_value->setStyleSheet(style);
}

ValuePane::~ValuePane()
{
    delete ui;
}

DoubleSpinBoxWithUnit *ValuePane::pane_value()
{
    return ui->pane_value;
}

void ValuePane::set_title(const QString &title)
{
    ui->pane_label->setText(title);
}
