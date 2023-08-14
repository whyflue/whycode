#define QTITLETEXT_H
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>

class QTitleText :public QWidget
{
private:
    QWidget *m_parent;
    QPoint m_pCursor;

public:
    QTitleText( QWidget *parent) ;

    bool clicked;
    QLabel* title_text;

    /*void on_btn_close_clicked();

    void on_btn_deploy_clicked();

    void on_btn_minimise_clicked();*/

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};
