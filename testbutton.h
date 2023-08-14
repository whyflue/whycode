#ifndef TESTBUTTON_H
#define TESTBUTTON_H
#include "QMainWindow"
#include "QPushButton"
#include "QHBoxLayout"
#include "QFrame"
#include "QSize"


class testbutton : public QMainWindow
{
    Q_OBJECT
private:

public:
    testbutton();

    QFrame *m_button_bg;
    QPushButton *text_button;
    QPushButton *icon_button;
    QHBoxLayout *m_buttonLayout;
    QSize *maximumsize;

    void setButtonText(QString text, QFont font);
    bool pressed;
    int mwidth;
private Q_SLOTS:
    void buttonPressEvent(bool pressed_arg);
};

#endif // TESTBUTTON_H
