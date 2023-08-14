#ifndef QCODEPAGE_H
#define QCODEPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QObjectList>
class QSyntaxStyle;
class QCompleter;
class QStyleSyntaxHighlighter;
class QCodeEditor;
class QFileInfo;

namespace Ui {
class QCodePage;
}

class QCodePage : public QWidget
{
    Q_OBJECT

public:
    explicit QCodePage(QWidget *parent = nullptr);
    ~QCodePage();

    void loadStyle(QString path);

    void initData();

    void createWidgets();

    bool Flag_IsNew;
    bool Flag_isOpen;

    QCodeEditor * m_codeEditor;
    QVector<QPair<QString, QString>> m_codeSamples;
    QVector<QPair<QString, QCompleter*>> m_completers;
    QVector<QPair<QString, QStyleSyntaxHighlighter*>> m_highlighters;
    QVector<QPair<QString, QSyntaxStyle*>> m_styles;

    QLineEdit *pathLine;
    QPushButton *btn_open;
    QPushButton *btn_run;
    QFrame *frame_codeedit;
    QFrame *frame_path;

    QObjectList *codechildren;
    QWidget *useless_layout;

/*public slots:

    void OpenFile();*/

private slots:

    void SaveFileAs();
    void SaveFile();
    void CreateNewFile();
    void OpenFile();

    void RunFile();

private:
    Ui::QCodePage *ui;
};

#endif // QCODEPAGE_H
