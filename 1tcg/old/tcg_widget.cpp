#include "tcg_widget.h"
#include "ui_tcg_widget.h"

#include <QMessageBox>
#include <QDebug>

CustomDelegate::CustomDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regExp_gain("^-?\\d*\\.?\\d{0,2}$");
    QRegExpValidator *validator = new QRegExpValidator(regExp_gain, editor);
    editor->setValidator(validator);
    return editor;
}

TCGWidget::TCGWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TCGWidget)
{
    ui->setupUi(this);

    ui->tableWidget_tcg->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_tcg->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_tcg->setEditTriggers(QAbstractItemView::DoubleClicked);

    QRegExp regExp_depth("^\\d*\\.?\\d{0,2}$");
    QRegExp regExp_gain("^-?\\d*\\.?\\d{0,2}$");
    QRegExpValidator* validator_depth = new QRegExpValidator(regExp_depth, ui->lineEdit_tcg_depth);
    QRegExpValidator* validator_gain = new QRegExpValidator(regExp_gain, ui->lineEdit_tcg_gain);
    ui->lineEdit_tcg_depth->setValidator(validator_depth);
    ui->lineEdit_tcg_gain->setValidator(validator_gain);

    connect(ui->tableWidget_tcg, &QTableWidget::cellChanged, this, [=](int row, int column){
        ui->tableWidget_tcg->blockSignals(true);

        if (column != 1) {
            ui->tableWidget_tcg->blockSignals(false);
            return;
        }

        g->insp_elect_tcg.gain_tcg[row] = ui->tableWidget_tcg->item(row, column)->text().toDouble();
        tcg_ok();

        ui->tableWidget_tcg->blockSignals(false);
    });

    ui->pushButton_tcg_ok->hide();
}

TCGWidget::~TCGWidget()
{
    delete ui;
}

void TCGWidget::set_param_g(in::ParamGroup* g) {

    this->g = g;
    update_table();
}

void TCGWidget::update_table() {
    ui->tableWidget_tcg->blockSignals(true);
    ui->tableWidget_tcg->setRowCount(0);
    for (int i = 0; i < g->insp_elect_tcg.depth_tcg.size(); i++) {
        ui->tableWidget_tcg->insertRow(i);
        ui->tableWidget_tcg->setItem(i, 0, new QTableWidgetItem(QString::number(g->insp_elect_tcg.depth_tcg.at(i) ) ) );
        ui->tableWidget_tcg->setItem(i, 1, new QTableWidgetItem(QString::number(g->insp_elect_tcg.gain_tcg.at(i) ) ) );

        ui->tableWidget_tcg->item(i, 0)->setFlags(Qt::ItemIsEnabled);
        CustomDelegate* delegate = new CustomDelegate(ui->tableWidget_tcg);
        ui->tableWidget_tcg->setItemDelegateForColumn(1, delegate);
    }
    ui->tableWidget_tcg->blockSignals(false);
}

void TCGWidget::on_lineEdit_tcg_depth_returnPressed() { emit ui->pushButton_tcg_add->released(); }

void TCGWidget::on_lineEdit_tcg_gain_returnPressed() { emit ui->pushButton_tcg_add->released(); }

void TCGWidget::on_pushButton_tcg_add_released() {
    ui->tableWidget_tcg->blockSignals(true);
    int rowCount  = ui->tableWidget_tcg->rowCount();
    QString tcg_depth = ui->lineEdit_tcg_depth->text();
    QString tcg_gain = ui->lineEdit_tcg_gain->text();

    if (rowCount  > 19) {
        QMessageBox::information(this, u8"提示", QString(u8"数目最多为%1!").arg(rowCount));
    } else if (tcg_depth.isEmpty()) {
        QMessageBox::information(this, u8"警告", u8"深度为空!");
    } else if (tcg_gain.isEmpty()) {
        QMessageBox::information(this, u8"警告", u8"增益为空!");
    } else {
        int index = rowCount;
        double new_depth = tcg_depth.toDouble();

        auto add_row = [this, rowCount, tcg_depth, tcg_gain](int index) {
            ui->tableWidget_tcg->insertRow(index);
            ui->tableWidget_tcg->setItem(index, 0, new QTableWidgetItem(tcg_depth));
            ui->tableWidget_tcg->setItem(index, 1, new QTableWidgetItem(tcg_gain));
            //滚动相应行
            if (index == rowCount) {
                ui->tableWidget_tcg->scrollToBottom();
            } else {
                ui->tableWidget_tcg->scrollToItem(ui->tableWidget_tcg->item(index, 0), QAbstractItemView::PositionAtBottom);
            }
            // 第一列不可编辑
            ui->tableWidget_tcg->item(index, 0)->setFlags(Qt::ItemIsEnabled);
            // 正则限制第二列最多为2位小数
            CustomDelegate* delegate = new CustomDelegate(ui->tableWidget_tcg);
            ui->tableWidget_tcg->setItemDelegateForColumn(1, delegate);
        };

        for (int i = 0; i < index; i++) {
            double existing_depth = ui->tableWidget_tcg->item(i, 0)->text().toDouble();
            if (std::abs(new_depth - existing_depth) <= 0.2) {
                index = -1;
                ui->tableWidget_tcg->item(i, 1)->setText(tcg_gain);
                ui->tableWidget_tcg->scrollToItem(ui->tableWidget_tcg->item(i, 0));
                break;
            }
            if (new_depth < existing_depth - 0.2) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            add_row(index);
            g->insp_elect_tcg.depth_tcg.insert(g->insp_elect_tcg.depth_tcg.begin() + index, tcg_depth.toDouble());
            g->insp_elect_tcg.gain_tcg.insert(g->insp_elect_tcg.gain_tcg.begin() + index, tcg_gain.toDouble());
        }
    }

    ui->lineEdit_tcg_depth->clear();
    ui->lineEdit_tcg_gain->clear();
    ui->tableWidget_tcg->blockSignals(false);
    tcg_ok();
}

void TCGWidget::on_pushButton_tcg_del_released() {
    int cnt_selected = ui->tableWidget_tcg->selectedItems().size();
    if (0 == cnt_selected) {
        qDebug() << "TCG Table: No rows selected.";
        return;
    }
    ui->tableWidget_tcg->blockSignals(true);
    g->insp_elect_tcg.depth_tcg.erase(g->insp_elect_tcg.depth_tcg.begin() + ui->tableWidget_tcg->currentRow());
    g->insp_elect_tcg.gain_tcg.erase(g->insp_elect_tcg.gain_tcg.begin() + ui->tableWidget_tcg->currentRow());
    update_table();
    ui->tableWidget_tcg->blockSignals(false);
    tcg_ok();
}

void TCGWidget::tcg_ok()
{
    emit trig_tcg_gain_changed();
}
