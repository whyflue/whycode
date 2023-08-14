#ifndef QEXPLORER_H
#define QEXPLORER_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QAbstractFileIconProvider>


class QExplorer : public QWidget
{

public:

    QExplorer( QWidget *parent);

    QFileSystemModel *dirmodel;
    QTreeView *treeView;
    QAbstractFileIconProvider *provider;


private:

    /*QFileSystemModel *dirmodel;
    QTreeView *treeView;
    QAbstractFileIconProvider *provider;*/

};

#endif // QEXPLORER_H
