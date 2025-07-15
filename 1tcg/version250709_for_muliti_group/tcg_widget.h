#ifndef TCG_WIDGET_H
#define TCG_WIDGET_H

#include <QWidget>

namespace Ui {
class TCGWidget;
}
class ParamManger;
struct ParamType1;
class TCGWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TCGWidget(QWidget *parent = nullptr);
    ~TCGWidget();
    void set_device_group_id(int device_id, int group_id);
    void set_param_manger(ParamManger *pm) {
        param_manger = pm;
    }
    void set_display_status(bool enable);

private slots:
    void on_btn_add_released();
    void on_btn_del_released();
    void on_comboBox_tcg_enable_currentIndexChanged(int index);

private:
    void init();
    void update_table();
    ParamType1 &get_type1();

private:
    Ui::TCGWidget *ui;
    int device_id = 0;
    int group_id = 0;
    ParamManger *param_manger;
};

#endif // TCG_WIDGET_H
