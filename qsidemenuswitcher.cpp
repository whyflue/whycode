#include "qsidemenuswitcher.h"
#include "QIcon"

QSideMenuSwitcher::QSideMenuSwitcher(QWidget * parent)

{
    sidemenuOFF = new QPushButton(this);
    sidemenuON = new QPushButton(this);
    sidemenuON->setMinimumSize(22,19);
    sidemenuON->setIconSize(QSize(22,19));
    sidemenuOFF->setIconSize(QSize(22,19));
    //connect(sidemenuON, SIGNAL(clicked(bool)), SLOT(switcherIconChange_clicked(bool)));
    //connect(sidemenuOFF, SIGNAL(clicked(bool)), SLOT(switcherIconChange_clicked(bool)));
    sidemenuON->setIcon(QIcon(":/resources/gui/icons/menu_off_selection_btn_selected.svg"));
    sidemenuOFF->setIcon(QIcon(":/resources/gui/icons/menu_on_selection_btn.svg"));

    switcher_bg = new QFrame(this);

    switcherbox = new QHBoxLayout(switcher_bg);
    switcherbox->addWidget(sidemenuOFF);
    switcherbox->addWidget(sidemenuON);
    bool menuPosition = true;

}
void QSideMenuSwitcher::switcherIconChange_clicked(bool menuPosition_arg)
{
    menuPosition_arg = menuPosition;
    if(menuPosition_arg == true){
        sidemenuON->setIcon(QIcon(":/resources/gui/icons/menu_off_selection_btn_selected.svg"));
        sidemenuOFF->setIcon(QIcon(":/resources/gui/icons/menu_on_selection_btn.svg"));
        menuPosition = false;
    }
    else{
        sidemenuON->setIcon(QIcon(":/resources/gui/icons/menu_off_selection_btn.svg"));
        sidemenuOFF->setIcon(QIcon(":/resources/gui/icons/menu_on_selection_btn_selected.svg"));
        menuPosition = true;
    }
}
