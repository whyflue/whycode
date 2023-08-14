#ifndef QPAGEBUTTON_H
#define QPAGEBUTTON_H
#include "QPushButton"
#include "QHBoxLayout"
#include "QString"
#include "QLabel"
#include "QFrame"
#include "QSize"
#include "QSpacerItem"


class QPageButton : public QWidget
{
    Q_OBJECT
private:
    QWidget *m_parent;

public:
    QPageButton(QWidget *parent);

    QFrame *m_button_bg;
    QPushButton *text_button;
    QPushButton *icon_button;
    QHBoxLayout *m_buttonLayout;
    QHBoxLayout *central_layout;
    QLabel *m_iconplacement;
    QSize *maximumsize;
    QSpacerItem *hspacer;

    void setButtonText(QString text, QFont font);
    void setButtonIcon(QString icon_path);
    bool pressed;
    int textbtn_width;


private Q_SLOTS:
    void buttonPressEvent(bool pressed);
    //void mousePressEvent(QMouseEvent *event, bool pressed_arg);


};

#endif // QPAGEBUTTON_H
