#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QFile>

int main(int argc, char *argv[])
{
     //ваыаываыва
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.setWindowTitle("WhyCode");
    w.setWindowIcon(QIcon(":/resources/gui/icons/icon_wclogo.svg"));
    w.show();
    return a.exec();
}
