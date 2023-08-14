#include "qexplorer.h"

#include "QString"
#include "QFontDatabase"
#include "QFileDialog"
#include "QMessageBox"

QExplorer::QExplorer(QWidget *parent)
{
    //QString sPath = "D:/";
    dirmodel = new QFileSystemModel(this);
    //dirmodel->setRootPath(sPath);
    treeView = new QTreeView(this);
    treeView->setModel(dirmodel);
    //treeView->setRootIndex(dirmodel->index(sPath));

    treeView->setStyleSheet("QTreeView{color: #ffffff;}"

                       "QTreeView::branch:has-siblings:adjoins-item {"
                            "border-image: none;"
                            "height: 6px;"
                            "width: 6px;"
                            "image: url(:/resources/gui/icons/corner.png);"
                        "}"

                        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
                            "border-image: none;"
                            "height: 6px;"
                            "width: 6px;"
                            "image: url(:/resources/gui/icons/icon_corner.png);"
                        "}"

                        "QTreeView::branch:has-children:!has-siblings:closed,"
                        "QTreeView::branch:closed:has-children:has-siblings {"
                                "border-image: none;"
                                "height: 8;"
                                "width: 8;"
                                "image: url(:/resources/gui/icons/arrow_closed.png);"
                        "}"

                        "QTreeView::branch:open:has-children:!has-siblings,"
                        "QTreeView::branch:open:has-children:has-siblings  {"
                                "border-image: none;"
                                "height: 8px;"
                                "width: 8px;"
                                "image: url(:/resources/gui/icons/arrow_more.png);"
                        "}");
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->hideColumn(3);

    treeView->setHeaderHidden(true);


    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(12);

    treeView->setFont(rubik);

    //provider = new QAbstractFileIconProvider(this);
}

