#include "qtitletext.h"
#include "QLabel"
#include "QHBoxLayout"
#include "QPushButton"
#include "QFontDatabase"


QTitleText::QTitleText(QWidget *parent ) :m_parent(parent)
{
    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(18);

    title_text = new QLabel("WhyCode");
    title_text->setFont(rubik);
    title_text->setFixedHeight(36);

    QFrame *title_content_back = new QFrame();
    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->addWidget(title_content_back);

    /*QPushButton *btn_close = new QPushButton();
    btn_close->setIcon(QIcon(":/resources/gui/icons/icon_close.svg"));
    QPushButton *btn_deploy = new QPushButton();
    btn_deploy->setIcon(QIcon(":/resources/gui/icons/icon_deploy.svg"));
    QPushButton *btn_minimise = new QPushButton();
    btn_minimise->setIcon(QIcon(":/resources/gui/icons/icon_minimise.svg"));
    connect(btn_minimise,SIGNAL(clicked()),parent,SLOT(on_btn_minimise_clicked()));*/


    QHBoxLayout *layout = new QHBoxLayout(title_content_back);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(title_text);
    clicked = false;
    /*layout->addWidget(btn_minimise);
    layout->addWidget(btn_deploy);
    layout->addWidget(btn_close);*/

}

void QTitleText::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pCursor = event->globalPos() - m_parent->geometry().topLeft();
        event->accept();
    }
}

void QTitleText::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        m_parent->move(event->globalPos() - m_pCursor);
        event->accept();
    }
}
void QTitleText::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(clicked == false)
        {
            m_parent->showMaximized();
            clicked = true;
        }
        else
        {
            m_parent->showNormal();
            clicked = false;
        }
    }
}
/*void QTitleText::on_btn_close_clicked()
{
    m_parent ->close();
}
bool clicked = false;
void QTitleText::on_btn_deploy_clicked()
{
    if(clicked == false)
    {
        m_parent->showMaximized();
        clicked = true;
    }
    else
    {
        m_parent->showNormal();
        clicked = false;
    }
}
void QTitleText::on_btn_minimise_clicked()
{
    m_parent->showMinimized();
}*/
