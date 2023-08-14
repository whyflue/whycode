#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QSyntaxStyle;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QCompleter;
class QStyleSyntaxHighlighter;
class QCodeEditor;
class QTitleText;
class QPageButton;
class QSideMenuSwitcher;
class QSettingsPage;
class QCodePage;
class QExplorer;
class QFrame;
class QVBoxLayout;
class QFont;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadStyle(QString path);

    QString loadCode(QString path);

    void initData();

    void createWidgets();

    void setupWidgets();

    void performConnections();

    QCodePage* codepage;
    QSettingsPage* settingspage;
    QComboBox* m_codeSampleCombobox;
    QComboBox* m_highlighterCombobox;
    QComboBox* m_completerCombobox;
    QComboBox* m_styleCombobox;

    QCheckBox* m_readOnlyCheckBox;
    QCheckBox* m_wordWrapCheckBox;
    QCheckBox* m_parenthesesEnabledCheckbox;
    QCheckBox* m_tabReplaceEnabledCheckbox;
    QSpinBox*  m_tabReplaceNumberSpinbox;
    QCheckBox* m_autoIndentationCheckbox;

    //QCodeEditor* m_codeEditor;
    QTitleText* title;
    QPageButton* settingsbutton;
    QPageButton* codepagebutton;
    QPageButton* whycombutton;
    QPageButton* codepagebutton2;
    QSideMenuSwitcher* menuswitcher;

    QExplorer *fileexplorer;
    QVBoxLayout *codeexplorer_layout;
    QFrame *codeexplorer_bg;

    QVector<QPair<QString, QString>> m_codeSamples;
    QVector<QPair<QString, QCompleter*>> m_completers;
    QVector<QPair<QString, QStyleSyntaxHighlighter*>> m_highlighters;
    QVector<QPair<QString, QSyntaxStyle*>> m_styles;
    QVector<QPair<QString, QSyntaxStyle*>> m_lightstyles;



private slots:
    void on_btn_donate_clicked();

    void on_btn_close_clicked();

    void on_btn_deploy_clicked();

    void on_btn_minimise_clicked();

    void settingsbutton_clicked();
    void codepagebutton_clicked();
    void whycombutton_clicked();

    void applysettings();

    void leftMenuClose(bool);

    void on_fileexplorerDoubleClicked(const QModelIndex &index);

    void slotTreeMenu(const QPoint &point);

    void OpenFile();


    /*void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //void misePressEvent(QMouseEvent *event);

    //void paintEvent(QPaintEvent *event);*/

private:
    //void initTitleBar();
    //void paintEvent(QPaintEvent *event);
    Ui::MainWindow *ui;
//protected:
    //QTitleBar* m_titleBar;
};
#endif // MAINWINDOW_H
