#ifndef TCG_WIDGET_H
#define TCG_WIDGET_H

#include "param_view_model.h"
#include <QTableWidget>
#include <QWidget>
#include <QItemDelegate>
#include <QDialog>

class CustomDelegate : public QItemDelegate {
    Q_OBJECT
public:
    CustomDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

namespace Ui {
class TCGWidget;
}

class TCGWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TCGWidget(QWidget *parent = nullptr);
    ~TCGWidget();

    void set_view_model(ParamViewModel *model);

public:
    void init();

private:
    void update_table();

private slots:
    void on_btn_add_released();
    void on_btn_del_released();

private:
    Ui::TCGWidget *ui;
    ParamViewModel *view_model;
};

#endif // TCG_WIDGET_H
