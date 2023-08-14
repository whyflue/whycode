#ifndef QSIDEMENUSWITCHER_H
#define QSIDEMENUSWITCHER_H
#include "QPushButton"
#include "QHBoxLayout"
#include "QFrame"
#include "QWidget"


class QSideMenuSwitcher : public QWidget
{
    Q_OBJECT

public:
    QSideMenuSwitcher(QWidget * parent);

    QPushButton *sidemenuON;

    QPushButton *sidemenuOFF;

    QHBoxLayout *switcherbox;

    QFrame *switcher_bg;
    bool menuPosition;

private Q_SLOTS:
    void switcherIconChange_clicked(bool menuPosition_arg);


};
#endif
