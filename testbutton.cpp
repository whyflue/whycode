#include "testbutton.h"
#include "QPropertyAnimation"

#include "QPushButton"
#include "QHBoxLayout"
#include "QString"
#include "QLabel"
#include "QFrame"
#include "QtSvg/QSvgRenderer"

testbutton::testbutton()
{
    text_button = new QPushButton(this);
    text_button->setText("example");
    text_button->setFixedHeight(39);
    text_button->setMinimumWidth(0);
    icon_button = new QPushButton(this);
    icon_button->setFixedHeight(39);
    icon_button->setFixedWidth(39);
    icon_button->setText("X");
    connect(text_button, SIGNAL(clicked(bool)), this, SLOT(buttonPressEvent(bool)));
    connect(icon_button, SIGNAL(clicked(bool)), this, SLOT(buttonPressEvent(bool)));

    m_button_bg = new QFrame(this);
    m_button_bg->setFixedHeight(39);
    int mwidth = m_button_bg->width();

    m_buttonLayout = new QHBoxLayout(m_button_bg);
    m_buttonLayout->addWidget(text_button);
    m_buttonLayout->addWidget(icon_button);
    bool pressed = false;
}
void testbutton::buttonPressEvent(bool pressed_arg){
    QPropertyAnimation *animation = new QPropertyAnimation(m_button_bg, "size", this);
    pressed_arg = pressed;
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::InCurve);
    int maxwidth = mwidth;
    if(pressed_arg == true)
        {
            animation->setStartValue(QSize(m_button_bg->width(), 39));
            animation->setEndValue(QSize(39, 39));
            pressed = false;
            animation->start();
            connect(animation, &QPropertyAnimation::finished, text_button, &QPushButton::hide );


         }
    else{
        animation->setStartValue(QSize(m_button_bg->width(), 39));
        animation->setEndValue(QSize(maxwidth, 39));
        pressed = true;
        text_button->show();
        animation->start();

    }
}
