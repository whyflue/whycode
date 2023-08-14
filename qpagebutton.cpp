#include "qpagebutton.h"
#include "QPropertyAnimation"

#include "QPushButton"
#include "QHBoxLayout"
#include "QString"
#include "QLabel"
#include "QFrame"
#include "QtSvg/QSvgRenderer"

QPageButton::QPageButton(QWidget * parent):m_parent(parent)
{
    text_button = new QPushButton(this);
    text_button->setFixedHeight(39);
    text_button->setMinimumWidth(0);
    text_button->setStyleSheet("QPushButton{color: white;}");

    icon_button = new QPushButton(this);
    icon_button->setFixedHeight(39);
    icon_button->setFixedWidth(39);

    //hspacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);

    //connect(text_button, SIGNAL(clicked(bool)), this, SLOT(buttonPressEvent(bool)));
    //connect(icon_button, SIGNAL(clicked(bool)), this, SLOT(buttonPressEvent(bool)));

    this->setContentsMargins(0, 0, 0, 0);
    central_layout = new QHBoxLayout();
    central_layout->setContentsMargins(0,0,0,0);
    this->setLayout(central_layout);
    m_button_bg = new QFrame();
    m_button_bg->setStyleSheet("border-radius: 5px; background-color: #272727;");
    m_button_bg->setFixedHeight(39);
    m_button_bg->setMinimumWidth(39);
    m_button_bg->setMaximumWidth(202);
    central_layout->addWidget(m_button_bg);

    m_buttonLayout = new QHBoxLayout(m_button_bg);
    m_buttonLayout->setSpacing(0);
    m_buttonLayout->addWidget(icon_button);
    m_buttonLayout->addWidget(text_button);
    //m_buttonLayout->addItem(hspacer);
    m_buttonLayout->setContentsMargins(0,0,0,0);

    bool pressed = true;

    m_button_bg->setMaximumWidth(m_button_bg->width());
}
void QPageButton::setButtonText(QString text, QFont font){
    text_button->setText(text);
    text_button->setFont(font);
}
void QPageButton::setButtonIcon(QString icon_path){
    icon_button->setIcon(QIcon(icon_path));
    icon_button->setIconSize(QSize(19, 19));
}
void QPageButton::buttonPressEvent(bool pressed_arg){
    QPropertyAnimation *animation = new QPropertyAnimation(text_button, "size", this);
    pressed_arg = pressed;
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InCurve);

    if(pressed_arg == true)
        {
            animation->setStartValue(QSize(text_button->width(), 39));
            animation->setEndValue(QSize(text_button->minimumWidth(), 39));
            pressed = false;
            animation->start();
            connect(animation, &QPropertyAnimation::finished, text_button, &QPushButton::hide );
            //qDebug("скрыта");


         }
    else{
        text_button->show();
        animation->setStartValue(QSize(text_button->width(), 39));
        animation->setEndValue(QSize(m_button_bg->width()-icon_button->width(), 39));
        pressed = true;
        animation->start();
        //qDebug("открыта");
    }
}
