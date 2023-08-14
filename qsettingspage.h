#ifndef QSETTINGSPAGE_H
#define QSETTINGSPAGE_H

#include <QWidget>
class QSyntaxStyle;
class QCompleter;
class QStyleSyntaxHighlighter;
class QCodeEditor;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QSettingsPage;
class QPushButton;

namespace Ui {
class QSettingsPage;
}

class QSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit QSettingsPage(QWidget *parent = nullptr);
    ~QSettingsPage();

    QComboBox* m_codeSampleCombobox;
    QComboBox* m_highlighterCombobox;
    QComboBox* m_completerCombobox;
    QComboBox* m_styleCombobox;
    QComboBox* themefile_combobox;

    QCheckBox* m_readOnlyCheckBox;
    QCheckBox* m_wordWrapCheckBox;
    QCheckBox* m_parenthesesEnabledCheckbox;
    QCheckBox* m_tabReplaceEnabledCheckbox;
    QSpinBox*  m_tabReplaceNumberSpinbox;
    QCheckBox* m_autoIndentationCheckbox;

    QPushButton* applySettingsButton;

    void loadStyle(QString path);

    QString loadCode(QString path);

    QString heading;
    int fontsize;
    QFont currentfont;
    int tabvalue;
    bool wordwrapPos;
    bool autoindetentionPos;
    bool btndonatePos;
    bool parenthesesPos;
    bool linenumbers;
    bool blueiconclosePos;

    int codetheme;

    bool resetsettings;

    void initData();

    void createWidgets();

    void setupWidgets();

    void performConnections();
    void getSettingsValues();

    //bool apply_pressed;

    QCodeEditor * m_codeEditor;
    QVector<QPair<QString, QString>> m_codeSamples;
    QVector<QPair<QString, QCompleter*>> m_completers;
    QVector<QPair<QString, QStyleSyntaxHighlighter*>> m_highlighters;
    QVector<QPair<QString, QSyntaxStyle*>> m_styles;
    QVector<QPair<QString, QSyntaxStyle*>> m_lightstyles;

    QWidget *settings_bg;

private slots:
    void resetall(bool clicked);

private:
    Ui::QSettingsPage *ui;
};

#endif // QSETTINGSPAGE_H
