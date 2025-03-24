#ifndef TCG_WIDGET_H
#define TCG_WIDGET_H

#include <QTableWidget>
#include <QWidget>
#include <QItemDelegate>

#include "parameter.h"

class CustomDelegate : public QItemDelegate {
    Q_OBJECT
public:
    CustomDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

namespace Ui {
class TCGWidget;
}

class TCGWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TCGWidget(QWidget *parent = nullptr);
    ~TCGWidget();

public:
    void set_param_g(in::ParamGroup* g);

signals:
    void trig_tcg_gain_changed();

private:
    void update_table();

private slots:
    void on_pushButton_tcg_add_released();
    void on_pushButton_tcg_del_released();
    void on_lineEdit_tcg_gain_returnPressed();
    void on_lineEdit_tcg_depth_returnPressed();
    void tcg_ok();

private:
    Ui::TCGWidget *ui;
    in::ParamGroup* g;
};

#endif // TCG_WIDGET_H
