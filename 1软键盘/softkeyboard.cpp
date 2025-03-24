#include "softkeyboard.h"
#include "ui_softkeyboard.h"
#include <QTimer>
#include <QThread>

/*
----------------------数字键盘和文字键盘--------------------
1.英文大小写和部分符号字符输入;
2.数字输入;
*/

SoftKeyboard::SoftKeyboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SoftKeyboard)
{
    ui->setupUi(this);

    //系统边框隐藏
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint
                   | Qt::WindowStaysOnTopHint);
    //系统窗口透明
    setAttribute(Qt::WA_TranslucentBackground, true);

    setMouseTracking(true);

    initButtonGroup();
    ui->Backspace->installEventFilter(this);
    this->installEventFilter(this);
    this->clickPos = QPoint(this->width() / 2, this->height() / 2);
}

SoftKeyboard::~SoftKeyboard()
{
    delete ui;
}

//获取目标输入对象
void SoftKeyboard::setTargetWidget(QWidget *w)
{
    InteritName = w->objectName();
    target = w;
    if (w->inherits("QLineEdit")) {
        ui->lineEdit_keyboard->setText(dynamic_cast<QLineEdit *>(w)->text());
        ui->stackedWidget->hide();

        ui->widget->hide();
        ui->Enlishwidget->show();
        this->move((1300 - this->width()) >> 1, 1130 - this->height());
        this->show();
    } else if (w->inherits("QDoubleSpinBox")) {
        QDoubleSpinBox *temp = dynamic_cast<QDoubleSpinBox *>(w);
        ui->cal_value->setValue(temp->value());
        ui->cal_value->setDecimals(temp->decimals());
        ui->stackedWidget->show();
        ui->Enlishwidget->hide();
        ui->stackedWidget->setCurrentIndex(1);
        QTimer::singleShot(0, ui->cal_value, &QDoubleSpinBox::selectAll);
        ui->step->setChecked(false);
        if (temp->objectName() == "gainValue") {
            GainStep = 1;
        } else {
            GainStep = 0;
        }
        //初始化
        if (temp->value() > 1000) {

            QString step = "1";
            QStringList stepStr = { "100", "10", "1", "0.1", "0.01" };
            stepStr.removeAt(stepStr.indexOf(step));
            ui->step->setText(u8"Δ" + step);
            ui->step->setChecked(false);
            for (int j = 0; j < 4; j++) {
                dialStepButtons->button(j)->setText(stepStr[j]);
            }
            stepValue = ui->step->text().remove(0, 1).toDouble();
            if (temp->decimals() == 0) {
                ui->step_3->setVisible(false);
                ui->step_4->setVisible(false);
            } else if (temp->decimals() == 1) {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(false);
            } else {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(true);
            }
        } else if (temp->value() >= 100 && temp->value() <= 1000) {
            QString step = "1";
            QStringList stepStr = { "100", "10", "1", "0.1", "0.01" };
            stepStr.removeAt(stepStr.indexOf(step));
            ui->step->setText(u8"Δ" + step);
            ui->step->setChecked(false);
            for (int j = 0; j < 4; j++) {
                dialStepButtons->button(j)->setText(stepStr[j]);
            }

            stepValue = ui->step->text().remove(0, 1).toDouble();
            if (temp->decimals() == 0) {
                ui->step_3->setVisible(false);
                ui->step_4->setVisible(false);
            } else if (temp->decimals() == 1) {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(false);
            } else {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(true);
            }
        } else if (temp->value() < 100) {
            QString step;
            QStringList stepStr;
            if (temp->objectName() == "gainValue") {
                stepStr.clear();
                step = "0.5";
                stepStr.clear();
                stepStr << "6"
                        << "1"
                        << "0.5"
                        << "0.1"
                        << "0.01";
            } else {
                stepStr.clear();
                step = "1";
                stepStr.clear();
                stepStr << "100"
                        << "10"
                        << "1"
                        << "0.1"
                        << "0.01";
            }

            stepStr.removeAt(stepStr.indexOf(step));
            ui->step->setText(u8"Δ" + step);
            ui->step->setChecked(false);
            for (int j = 0; j < 4; j++) {
                dialStepButtons->button(j)->setText(stepStr[j]);
            }
            stepValue = ui->step->text().remove(0, 1).toDouble();
            if (temp->decimals() == 0) {
                ui->step_3->setVisible(false);
                ui->step_4->setVisible(false);
            } else if (temp->decimals() == 1) {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(false);
            } else {
                ui->step_3->setVisible(true);
                ui->step_4->setVisible(true);
            }
        }
        this->move((3020 - this->width()) >> 1, 1080 - this->height());
        this->show();
    }
}

void SoftKeyboard::saveParent(QWidget *parent)
{
    sparent = parent;
}

QWidget *SoftKeyboard::getDbox()
{
    return ui->cal_value;
}

//处理回车事件，和大小写转换、字符转换
void SoftKeyboard::keyPressEvent(QKeyEvent *e)
{

    if (e->key() == Qt::Key_CapsLock) {

        QString temp = buttons->button(0)->text();
        for (int i = 0; i < 26; ++i) {
            if (temp.isLower()) {
                buttons->button(i)->setText(buttons->button(i)->text().toUpper());
            } else {
                buttons->button(i)->setText(buttons->button(i)->objectName().at(4).toLower());
            }
        }
    } /* else if (e->key() == Qt::Key_Shift) {
         for (int i = 0; i < 26; ++i) {
             buttons->button(i)->setText(this->symbol[i]);
         }
     }*/
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (target->inherits("QDoubleSpinBox")) {
            dynamic_cast<QDoubleSpinBox *>(target)->setValue(ui->cal_value->value());
            emit dynamic_cast<QDoubleSpinBox *>(target)->editingFinished();
        } else if (target->inherits("QLineEdit")) {
            dynamic_cast<QLineEdit *>(target)->setText(ui->lineEdit_keyboard->text());
        }
        this->close();
    } else if (e->key() == Qt::Key_Escape) {
        this->close();
    } else if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) {
        if (target->inherits("QDoubleSpinBox")) {
            dynamic_cast<QDoubleSpinBox *>(target)->setValue(ui->cal_value->value());
            emit dynamic_cast<QDoubleSpinBox *>(target)->valueChanged(ui->cal_value->value());
        }
    }
}

//窗口移动
void SoftKeyboard::mousePressEvent(QMouseEvent *e)
{
    m_presstime = QTime::currentTime();
    if (e->button() == Qt::LeftButton) {
        clickPos = e->pos();
    }
}

//窗口移动
void SoftKeyboard::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {
        move(e->pos() + pos() - clickPos);
    }
}
//窗口松开
void SoftKeyboard::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
    }
}

//主窗口失去焦点隐藏
bool SoftKeyboard::eventFilter(QObject *obj, QEvent *event)
{
    //长按与单击事件
    if (obj == ui->Backspace) {
        if (event->type() == QEvent::MouseButtonPress) {
            m_presstime.restart();
            m_presstime = QTime::currentTime();
        }
        if (event->type() == QEvent::MouseButtonRelease) {
            int elapsed = m_presstime.msecsTo(QTime::currentTime());
            if (elapsed >= 800) {
                ui->lineEdit_keyboard->clear();
            } else {
                ui->lineEdit_keyboard->backspace();
            }
        }
    }
    if (event->type() == QEvent::ActivationChange) {
        if (QApplication::activeWindow() != this) {
            ui->lineEdit_keyboard->clear();
            this->hide();
            if (InteritName == "valueX" || InteritName == "valueY") {
                emit MoveShowActive();
            } else {
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void SoftKeyboard::hideEvent(QHideEvent *event)
{
    emit hidden(true);
    QWidget::hideEvent(event);
}

void SoftKeyboard::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    if (sparent != nullptr)
        sparent->setFocus();
}

void SoftKeyboard::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

//初始化键盘按钮组
void SoftKeyboard::initButtonGroup()
{
    buttons = new QButtonGroup;
    dialStepButtons = new QButtonGroup;

    buttons->addButton(ui->key_A, 0);
    buttons->addButton(ui->key_B, 1);
    buttons->addButton(ui->key_C, 2);
    buttons->addButton(ui->key_D, 3);
    buttons->addButton(ui->key_E, 4);
    buttons->addButton(ui->key_F, 5);
    buttons->addButton(ui->key_G, 6);
    buttons->addButton(ui->key_H, 7);
    buttons->addButton(ui->key_I, 8);
    buttons->addButton(ui->key_J, 9);
    buttons->addButton(ui->key_K, 10);
    buttons->addButton(ui->key_L, 11);
    buttons->addButton(ui->key_M, 12);
    buttons->addButton(ui->key_N, 13);
    buttons->addButton(ui->key_O, 14);
    buttons->addButton(ui->key_P, 15);
    buttons->addButton(ui->key_Q, 16);
    buttons->addButton(ui->key_R, 17);
    buttons->addButton(ui->key_S, 18);
    buttons->addButton(ui->key_T, 19);
    buttons->addButton(ui->key_U, 20);
    buttons->addButton(ui->key_V, 21);
    buttons->addButton(ui->key_W, 22);
    buttons->addButton(ui->key_X, 23);
    buttons->addButton(ui->key_Y, 24);
    buttons->addButton(ui->key_Z, 25);

    buttons->addButton(ui->key_1);
    buttons->addButton(ui->key_2);
    buttons->addButton(ui->key_3);
    buttons->addButton(ui->key_4);
    buttons->addButton(ui->key_5);
    buttons->addButton(ui->key_6);
    buttons->addButton(ui->key_7);
    buttons->addButton(ui->key_8);
    buttons->addButton(ui->key_9);
    buttons->addButton(ui->key_0);
    buttons->addButton(ui->key_Point);
    buttons->addButton(ui->key_Minus);
    buttons->addButton(ui->key_Enter);
    buttons->addButton(ui->key_Clear);
    buttons->addButton(ui->key_Add);
    buttons->addButton(ui->key_Sub);

    buttons->addButton(ui->Backspace);
    buttons->addButton(ui->Enter);
    buttons->addButton(ui->CapsLock);
    buttons->addButton(ui->Space);
    buttons->addButton(ui->key_Left);
    buttons->addButton(ui->key_Right);
    buttons->addButton(ui->Point);
    buttons->addButton(ui->Comma);
    buttons->addButton(ui->Underline);
    //    buttons->addButton(ui->Symbol);

    //    dialStepButtons->addButton(ui->step);
    dialStepButtons->addButton(ui->step_1, 0);
    dialStepButtons->addButton(ui->step_2, 1);
    dialStepButtons->addButton(ui->step_3, 2);
    dialStepButtons->addButton(ui->step_4, 3);

    connect(buttons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this,
            &SoftKeyboard::letterKeyClicked);
    connect(dialStepButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this,
            &SoftKeyboard::stepButtonsClicked);

    symbol << "!"
           << "<"
           << "*"
           << "^"
           << "3"
           << "&&"
           << "|"
           << "("
           << "8"
           << ")"
           << ":"
           << ";"
           << "="
           << ">"
           << "9"
           << "0"
           << "1"
           << "4"
           << "%"
           << "5"
           << "7"
           << "/"
           << "2"
           << "-"
           << "6"
           << "+";

    for (int i = 10; i >= 0; --i) {
        numList << i;
        numList << 110 + i;
    }
    if (ui->cal_value->value() > 1000) {
        stepValue = 100;
    } else if (ui->cal_value->value() >= 100 && ui->cal_value->value() <= 1000) {
        stepValue = 10;
    } else if (ui->cal_value->value() < 100) {
        stepValue = 1;
    }

    on_step_toggled(false);
}

//按键点击创建对应键盘事件
void SoftKeyboard::letterKeyClicked(QAbstractButton *button)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(button);
    QKeyEvent *event = nullptr;

    if (btn->objectName().contains("key_Clear")) {
        ui->cal_value->clear();
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Clear, Qt::NoModifier);
    } else if (btn->objectName().contains("Backspace")) {
        //        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Clear, Qt::NoModifier);

    } else if (btn->objectName().contains("Enter")) {
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
    } else if (btn->objectName() == "CapsLock") {
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_CapsLock, Qt::NoModifier);
    } else if (btn->objectName() == "Symbol") {
        //        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Shift, Qt::NoModifier);
    } else if (btn->objectName() == "key_Left") {
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    } else if (btn->objectName() == "key_Right") {
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    } else if (btn->objectName() == "Space") {
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, " ");
    } else if (btn->objectName() == "key_Add") {
        ui->cal_value->setValue(ui->cal_value->value() + stepValue);
        // event = new QKeyEvent(QKeyEvent::KeyPress, btn->text()[0].toLatin1(), Qt::NoModifier,
        //                      ui->doubleSpinBox->text());
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    } else if (btn->objectName() == "key_Sub") {
        ui->cal_value->setValue(ui->cal_value->value() - stepValue);
        // event = new QKeyEvent(QKeyEvent::KeyPress, btn->text()[0].toLatin1(), Qt::NoModifier,
        //                      ui->doubleSpinBox->text());
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    } else if (btn->objectName() == "key_Minus") {
        ui->cal_value->setValue(ui->cal_value->value() * -1);
        return;
    } else {
        event = new QKeyEvent(QKeyEvent::KeyPress, btn->text()[0].toLatin1(), Qt::NoModifier,
                              btn->text().at(0));
    }

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Shift)
        QApplication::postEvent(this, event);
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        QApplication::postEvent(this, event);
    } else if (target->inherits("QLineEdit"))
        QApplication::postEvent(ui->lineEdit_keyboard, event);
    else if (target->inherits("QDoubleSpinBox")) {
        QApplication::postEvent(ui->cal_value, event);
    }
}

//步径按钮值切换
void SoftKeyboard::stepButtonsClicked(QAbstractButton *button)
{
    stepValue = button->text().toDouble();
    QString step = button->text();
    QStringList stepStr;
    if (GainStep) {
        stepStr.clear();
        stepStr << "6"
                << "1"
                << "0.5"
                << "0.1"
                << "0.01";
    } else {
        stepStr.clear();
        stepStr << "100"
                << "10"
                << "1"
                << "0.1"
                << "0.01";
    }
    stepStr.removeAt(stepStr.indexOf(step));
    ui->step->setText(u8"Δ" + step);
    ui->step->setChecked(false);
    emit setCDFrame_stepvalue(step.toDouble());
    for (int j = 0; j < 4; j++) {
        dialStepButtons->button(j)->setText(stepStr[j]);
    }
}

void SoftKeyboard::on_step_toggled(bool checked)
{
    if (checked) {
        ui->widget->show();
    } else {
        ui->widget->hide();
    }
}

void SoftKeyboard::on_Symbol_clicked()
{
    if (ui->Symbol->text() == "abc") {
        ui->Symbol->setText("!&&%");
        for (int i = 0; i < 26; ++i) {
            buttons->button(i)->setText(this->symbol[i]);
        }
    } else {
        ui->Symbol->setText("abc");
        for (int i = 0; i < 26; ++i) {
            buttons->button(i)->setText(buttons->button(i)->objectName().at(4).toLower());
        }
    }
}
