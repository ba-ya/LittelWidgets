#include "tcg_widget.h"
#include "ui_tcg_widget.h"
#include "param_manger.h"

#include <QMessageBox>

TCGWidget::TCGWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCGWidget)
{
    ui->setupUi(this);
    init();
}

TCGWidget::~TCGWidget()
{
    delete ui;
}

void TCGWidget::set_device_group_id(int device_id, int group_id) {
    auto &param_type = param_manger->get_param(device_id, group_id);
    if (!std::holds_alternative<ParamType1>(param_type)) {
        return;
    }
    this->device_id = device_id;
    this->group_id = group_id;
    update_table();
}

void TCGWidget::set_display_status(bool enable)
{
    ui->comboBox_tcg_enable->setEnabled(enable);
    ui->table->setEnabled(enable);
    ui->lineEdit_tcg_depth->setEnabled(enable);
    ui->lineEdit_tcg_gain->setEnabled(enable);
    ui->btn_add->setEnabled(enable);
    ui->btn_del->setEnabled(enable);
}

void TCGWidget::init()
{
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->verticalHeader()->hide();
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QRegularExpression regex_depth(R"(^[+]?[0-9]*\.?[0-9]{0,2}$)");
    QRegularExpression regex_gain(R"(^[+-]?[0-9]*\.?[0-9]{0,2}$)");
    QValidator* validator_depth = new QRegularExpressionValidator(regex_depth, ui->lineEdit_tcg_depth);
    QValidator* validator_gain = new QRegularExpressionValidator(regex_gain, ui->lineEdit_tcg_gain);
    ui->lineEdit_tcg_depth->setValidator(validator_depth);
    ui->lineEdit_tcg_gain->setValidator(validator_gain);
}

void TCGWidget::update_table()
{
    ui->table->blockSignals(true);
    ui->table->setRowCount(0);
    auto [enable, depths, gains] = param_manger->get_tcg_prop(get_type1());
    ui->comboBox_tcg_enable->setCurrentIndex(enable);
    for (int i = 0; i < depths.size(); i++) {
        ui->table->insertRow(i);
        ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(depths.at(i))));
        ui->table->setItem(i, 1, new QTableWidgetItem(QString::number(gains.at(i))));
    }
    ui->table->blockSignals(false);
}

ParamType1 &TCGWidget::get_type1()
{
    auto &param_type = param_manger->get_param(device_id, group_id);
    return std::get<ParamType1>(param_type);
}

void TCGWidget::on_btn_add_released()
{
    int rowCount  = ui->table->rowCount();
    QString tcg_depth = ui->lineEdit_tcg_depth->text();
    QString tcg_gain = ui->lineEdit_tcg_gain->text();
    int max_cnt = 19;
    QString str_notice = u8"提示";
    if (rowCount > max_cnt) {
        QMessageBox::information(this, str_notice, QString(QObject::tr("您最多只能添加%1行!")).arg(max_cnt));
    } else if (tcg_depth.isEmpty()) {
        QMessageBox::information(this, str_notice, QString(QObject::tr("深度为空!")));
    } else if (tcg_gain.isEmpty()) {
        QMessageBox::information(this, str_notice, QString(QObject::tr("增益为空!")));
    } else {
    param_manger->add_tcg_point(get_type1(), tcg_depth.toDouble(), tcg_gain.toDouble());
        update_table();
        ui->lineEdit_tcg_depth->clear();
        ui->lineEdit_tcg_gain->clear();
    }
}


void TCGWidget::on_btn_del_released()
{
    auto selected = ui->table->selectedItems();
    if (0 == selected.size()) {
        QMessageBox::information(this, u8"删除", u8"未选中文件!");
        return;
    }

    // remove
    for (auto &item : selected) {
        auto dep = ui->table->item(item->row(), 0)->text().toDouble();
        param_manger->remove_tcg_point(get_type1(), dep);
    }
    update_table();
}


void TCGWidget::on_comboBox_tcg_enable_currentIndexChanged(int index)
{
    param_manger->set_tcg_status(get_type1(), index);
}

