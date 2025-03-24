#ifndef SOFTKEYBOARD_H
#define SOFTKEYBOARD_H

#include <QWidget>
#include <QButtonGroup>
#include <QString>
#include <QCharRef>

#include <QKeyEvent>
#include <QDoubleSpinBox>
#include <QTime>
#include <QStyle>

namespace Ui {
class SoftKeyboard;
}

class SoftKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit SoftKeyboard(QWidget *parent = nullptr);
    ~SoftKeyboard() override;

    //    enum DialDirection{
    //        ClockWise,
    //        CounterClockWise
    //    };

    void setTargetWidget(QWidget *);
    void saveParent(QWidget *);
    QWidget *getDbox(void);

protected:
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    //    void focusInEvent(QFocusEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *e) override;
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void initButtonGroup();

private slots:
    void letterKeyClicked(QAbstractButton *);
    void stepButtonsClicked(QAbstractButton *);

    // 暂时屏蔽键盘旋扭功能 end
    void on_step_toggled(bool checked);

    void on_Symbol_clicked();

private:
    Ui::SoftKeyboard *ui;

    QWidget *sparent;
    QButtonGroup *buttons;
    QButtonGroup *dialStepButtons;

    QString InteritName;

    double stepValue;
    QWidget *target;
    QStringList symbol; //字符
    QPoint clickPos;    //鼠标点击坐标
    QPoint movePos;     //鼠标移动坐标

    QList<int> numList;
    int before = 0;
    bool status = true;
    QTime m_presstime;
    int GainStep = 0;

signals:
    void hidden(bool);
    void MoveShowActive();
    void setCDFrame_stepvalue(double);
};

#endif // SOFTKEYBOARD_H
