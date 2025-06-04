#include "tcg_widget.h"
#include "ui_tcg_widget.h"

#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include "tr_strings.h"

CustomDelegate::CustomDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QRegularExpression regex_gain(R"(^[+-]?[0-9]*\.?[0-9]{0,2}$)");
    QValidator* validator_gain = new QRegularExpressionValidator(regex_gain, editor);
    editor->setValidator(validator_gain);
    return editor;
}

TCGWidget::TCGWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TCGWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("TCG Widget"));
    init();

}
TCGWidget::~TCGWidget()
{
    delete ui;
}

void TCGWidget::init()
{
    setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QRegularExpression regex_depth(R"(^[+]?[0-9]*\.?[0-9]{0,2}$)");
    QRegularExpression regex_gain(R"(^[+-]?[0-9]*\.?[0-9]{0,2}$)");
    QValidator* validator_depth = new QRegularExpressionValidator(regex_depth, ui->lineEdit_tcg_depth);
    QValidator* validator_gain = new QRegularExpressionValidator(regex_gain, ui->lineEdit_tcg_gain);
    ui->lineEdit_tcg_depth->setValidator(validator_depth);
    ui->lineEdit_tcg_gain->setValidator(validator_gain);

    connect(ui->table, &QTableWidget::cellChanged, this, [=](int row, int column){
        if (column != 1) {
            return;
        }
        auto dep = ui->table->item(row, 0)->text().toDouble();
        auto gain = ui->table->item(row, 1)->text().toDouble();
        view_model->add_tcg_point(dep, gain);
        update_table();
    });
}

void TCGWidget::set_view_model(ParamViewModel *model)
{
    view_model = model;
    update_table();
}

void TCGWidget::update_table() {
    ui->table->blockSignals(true);
    ui->table->setRowCount(0);
    auto [depths, gains] = view_model->get_tcg_prop();
    for (int i = 0; i < depths.size(); i++) {
        ui->table->insertRow(i);
        ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(depths.at(i))));
        ui->table->setItem(i, 1, new QTableWidgetItem(QString::number(gains.at(i))));

        ui->table->item(i, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        CustomDelegate* delegate = new CustomDelegate(ui->table);
        ui->table->setItemDelegateForColumn(1, delegate);
    }
    ui->table->blockSignals(false);
}

void TCGWidget::on_btn_add_released() {
    int rowCount  = ui->table->rowCount();
    QString tcg_depth = ui->lineEdit_tcg_depth->text();
    QString tcg_gain = ui->lineEdit_tcg_gain->text();

    if (rowCount > 19) {
        QMessageBox::information(this, TrStrings::title_notice(), QString(QObject::tr(u8"Max row count is %1!")).arg(rowCount));
    } else if (tcg_depth.isEmpty()) {
        QMessageBox::information(this, TrStrings::title_notice(), QString(QObject::tr(u8"Depth is empty!")));
    } else if (tcg_gain.isEmpty()) {
        QMessageBox::information(this, TrStrings::title_notice(), QString(QObject::tr(u8"Gain is empty!")));
    } else {
        view_model->add_tcg_point(tcg_depth.toDouble(), tcg_gain.toDouble());
        update_table();
    }

    ui->lineEdit_tcg_depth->clear();
    ui->lineEdit_tcg_gain->clear();
}

void TCGWidget::on_btn_del_released() {
    auto selected = ui->table->selectedItems();
    if (0 == selected.size()) {
        QMessageBox::information(this, TrStrings::title_delete(), TrStrings::msg_no_files_selected());
        return;
    }
    for (auto &item : selected) {
        auto dep = ui->table->item(item->row(), 0)->text().toDouble();
        view_model->remove_tcg_point(dep);
    }
    update_table();
}
