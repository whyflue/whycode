#include "qsettingspage.h"
#include "qlinenumberarea.h"
#include "ui_qsettingspage.h"
#include "qcodeeditor.h"
#include "qsyntaxstyle.h"
#include "qcxxhighlighter.h"
#include "qglslhighlighter.h"
#include "qxmlhighlighter.h"
#include "qjsonhighlighter.h"
#include "qluahighlighter.h"
#include "qpythonhighlighter.h"
#include "qglslcompleter.h"
#include "qluacompleter.h"
#include "qpythoncompleter.h"

#include <QDesktopServices>
#include <QUrl>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QFile>
#include <QCheckBox>
#include <QPainter>
#include <QPainterPath>
#include <QAbstractItemView>

QSettingsPage::QSettingsPage(QWidget *parent) :
    QWidget(parent),
    m_completerCombobox(nullptr),
    m_styleCombobox(nullptr),
    m_readOnlyCheckBox(nullptr),
    //m_wordWrapCheckBox(nullptr),
    m_parenthesesEnabledCheckbox(nullptr),
    m_tabReplaceEnabledCheckbox(nullptr),
    m_tabReplaceNumberSpinbox(nullptr),
    m_autoIndentationCheckbox(nullptr),
    //m_codeEditor(nullptr),
    m_completers(),
    m_highlighters(),
    m_styles(),
    ui(new Ui::QSettingsPage)
{
    ui->setupUi(this);
    initData();
    createWidgets();
    setupWidgets();
    performConnections();

    settings_bg = ui->settings_bg;
}

void QSettingsPage::initData()
{
    m_codeSamples = {
            {"C++",  loadCode(":/resources/code_samples/cxx.cpp")},
            {"GLSL", loadCode(":/resources/code_samples/shader.glsl")},
            {"XML",  loadCode(":resources/code_samples/xml.xml")},
            {"JSON",  loadCode(":/resources/code_samples/json.json")},
            {"LUA",  loadCode(":/resources/code_samples/lua.lua")},
            {"Python",  loadCode(":/resources/code_samples/python.py")}
        };

    m_completers = {
        {"None", nullptr},
        {"GLSL", new QGLSLCompleter(this)},
        {"LUA", new QLuaCompleter(this)},
        {"Python", new QPythonCompleter(this)},
    };

    m_highlighters = {
        //{"None", nullptr},
        {"C++",  new QCXXHighlighter},
        {"GLSL", new QGLSLHighlighter},
        {"XML",  new QXMLHighlighter},
        {"JSON", new QJSONHighlighter},
        {"LUA",  new QLuaHighlighter},
        {"Python",  new QPythonHighlighter},
    };

    m_styles = {
        {"default.xml", QSyntaxStyle::defaultStyle()},
        {"psicho.xml", QSyntaxStyle::psichoStyle()}
    };
    m_lightstyles = {
        {"default.xml", QSyntaxStyle::defaultLight()}
    };

    // Loading styles
    loadStyle(":recources/gui/themes/styles/drakula.xml");
    loadStyle(":recources/gui/themes/styles/psicho.xml");
    loadStyle(":recources/gui/themes/styles/default_whylightmode.xml");

    //heading = ui->heading_lineEdit->text();

}

QString QSettingsPage::loadCode(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return QString();
    }

    return fl.readAll();
}

void QSettingsPage::loadStyle(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return;
    }

    auto style = new QSyntaxStyle(this);

    if (!style->load(fl.readAll()))
    {
        delete style;
        return;
    }

    m_styles.append({style->name(), style});
}

void QSettingsPage::createWidgets()
{
    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(16);

    ui->settings_bg->setFont(rubik);

    themefile_combobox = ui->themefile_combobox;

    //Apply Button
    applySettingsButton = ui->applySettingsButton;
    applySettingsButton->setFont(QFont(rubik));

    // CodeEditor
    m_codeEditor = new QCodeEditor(this);
    ui->codeexample_layout->addWidget(m_codeEditor);
    m_codeEditor->setFont(QFont(rubik));
    m_codeEditor->setWordWrapMode(QTextOption::NoWrap);
    m_codeEditor->setReadOnly(true);
    QString scrollbar_theme = "QScrollBar:vertical {"
                                        "background: transparent;"         /*   убрать эту белую сетку в скроллбаре                 */
                                        "width: 14px;"
                                        "margin: 16px 2px 16px 2px;"
                                        "border-radius: 5px;"
                                    "}"


                                    "QScrollBar::handle:vertical {"
                                        "background-color: #4D545B;"
                                        "min-height: 30px;"
                                        "border-radius: 5px;"
                                    "}"

                                   " QScrollBar::handle:vertical:hover {"
                                        "background-color: #6A8AC8;"
                                        /*"border: 1px solid #6A8AC8;"*/
                                        "border-radius: 5px;"
                                    "}"

                                    "QScrollBar::sub-line:vertical {"
                                        "margin: 3px 0px 3px 0px;"
                                        "border-image: url(icons/up_arrow_disabled.png);"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: top;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::add-line:vertical {"
                                        "margin: 3px 0px 3px 0px;"
                                        "border-image: url(icons/down_arrow_disabled.png);"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: bottom;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::sub-line:vertical:hover,"
                                    "QScrollBar::sub-line:vertical:on {"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: top;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::add-line:vertical:hover,"
                                    "QScrollBar::add-line:vertical:on {"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: bottom;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::up-arrow:vertical,"
                                    "QScrollBar::down-arrow:vertical {"
                                        "background: none;"
                                    "}"

                                    "QScrollBar::add-page:vertical,"
                                    "QScrollBar::sub-page:vertical {"
                                        "background: none; "
                                    "}"


                                "/*---------------------------------------*/"
                                    "QScrollBar:horizontal"
                                    "{"
                                         "margin: 2px 16px 2px 16px; "
                                         "background: transparent;"
                                         "height: 14px;"
                                         "border-radius: 5px;"
                                    "}"

                                    "QScrollBar::handle:horizontal"
                                    "{"
                                         "background-color: #4D545B;"
                                         "border: 1px solid #31363B;"
                                         "min-width: 8px;"
                                         "border-radius: 5px;"
                                    "}"
                                    " QScrollBar::handle:horizontal:hover {"
                                         "background-color: #6A8AC8;"
                                         "/*border: 1px solid #6A8AC8;*/"
                                         "border-radius: 5px;"
                                         "min-width: 8px;"
                                    "}"

                                    "QScrollBar::add-line:horizontal"
                                    "{"
                                        "margin: 0px 3px 0px 3px;"
                                        "border-image: url(:/qss_icons/rc/right_arrow_disabled.png);"
                                        "width: 10px;"
                                        "height: 10px;"
                                        "subcontrol-position: right;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::sub-line:horizontal"
                                    "{"
                                        "margin: 0px 3px 0px 3px;"
                                        "border-image: url(:/qss_icons/rc/left_arrow_disabled.png);"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: left;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::add-line:horizontal:hover,QScrollBar::add-line:horizontal:on"
                                    "{"
                                        "/*border-image: url(:/qss_icons/rc/right_arrow.png);*/"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: right;"
                                        "subcontrol-origin: margin;"
                                    "}"


                                    "QScrollBar::sub-line:horizontal:hover, QScrollBar::sub-line:horizontal:on"
                                    "{"
                                        "/*border-image: url(:/qss_icons/rc/left_arrow.png);*/"
                                        "height: 10px;"
                                        "width: 10px;"
                                        "subcontrol-position: left;"
                                        "subcontrol-origin: margin;"
                                    "}"

                                    "QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal"
                                    "{"
                                        "background: none;"
                                    "}"


                                    "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal"
                                    "{"
                                        "background: none;"
                                    "}";
    m_codeEditor->setStyleSheet(scrollbar_theme);
    //ui->fontComboBox->setStyleSheet("QFontComboBox:down-arrow{image: url(\":/resorces/icons/icon_combobox_arrow\");}");
    m_codeSampleCombobox  = new QComboBox();
    m_highlighterCombobox = new QComboBox();
    m_completerCombobox   = new QComboBox();
    m_styleCombobox       = new QComboBox();

    m_readOnlyCheckBox           = new QCheckBox("Read Only");
    m_wordWrapCheckBox           = new QCheckBox("Word Wrap");
    m_parenthesesEnabledCheckbox = new QCheckBox("Auto Parentheses");
    m_tabReplaceEnabledCheckbox  = new QCheckBox("Tab Replace");
    m_tabReplaceNumberSpinbox    = new QSpinBox();
    m_autoIndentationCheckbox    = new QCheckBox("Auto Indentation");


    /*// Adding widgets
    m_setupLayout->addWidget(new QLabel(tr("Code sample"), setupGroup));
    m_setupLayout->addWidget(m_codeSampleCombobox);
    m_setupLayout->addWidget(new QLabel(tr("Completer"), setupGroup));
    m_setupLayout->addWidget(m_completerCombobox);
    m_setupLayout->addWidget(new QLabel(tr("Highlighter"), setupGroup));
    m_setupLayout->addWidget(m_highlighterCombobox);
    m_setupLayout->addWidget(new QLabel(tr("Style"), setupGroup));
    m_setupLayout->addWidget(m_styleCombobox);
    m_setupLayout->addWidget(m_readOnlyCheckBox);
    m_setupLayout->addWidget(m_wordWrapCheckBox);
    m_setupLayout->addWidget(m_parenthesesEnabledCheckbox);
    m_setupLayout->addWidget(m_tabReplaceEnabledCheckbox);
    m_setupLayout->addWidget(m_tabReplaceNumberSpinbox);
    m_setupLayout->addWidget(m_autoIndentationCheckbox);
    m_setupLayout->addSpacerItem(new QSpacerItem(1, 2, QSizePolicy::Minimum, QSizePolicy::Expanding));*/
}

void QSettingsPage::setupWidgets()
{

    // CodeEditor
    m_codeEditor->setPlainText  (m_codeSamples[0].second);
    m_codeEditor->setSyntaxStyle(m_styles[0].second);
    m_codeEditor->setCompleter  (m_completers[0].second);
    m_codeEditor->setHighlighter(m_highlighters[0].second);

    QStringList list;
    // Code samples
    for (auto&& el : m_codeSamples)
    {
        list << el.first;
    }

    ui->codelanguage_combobox->addItems(list);
    list.clear();

    // Highlighter
    for (auto&& el : m_highlighters)
    {
        list << el.first;
    }

    m_highlighterCombobox->addItems(list);
    list.clear();

    // Completer
    for (auto&& el : m_completers)
    {
        list << el.first;
    }

    m_completerCombobox->addItems(list);
    list.clear();

    // Styles
    for (auto&& el : m_styles)
    {
        list << el.first;
    }

    themefile_combobox->addItems(list);
    list.clear();

    m_parenthesesEnabledCheckbox->setChecked(m_codeEditor->autoParentheses());
    m_tabReplaceEnabledCheckbox->setChecked(m_codeEditor->tabReplace());
    m_tabReplaceNumberSpinbox->setValue(m_codeEditor->tabReplaceSize());
    m_tabReplaceNumberSpinbox->setSuffix(tr(" spaces"));
    m_autoIndentationCheckbox->setChecked(m_codeEditor->autoIndentation());

    m_wordWrapCheckBox->setChecked(m_codeEditor->wordWrapMode() != QTextOption::NoWrap);

    //DropDown of specifical comboboxes
    QAbstractItemView *view = ui->codelanguage_combobox->view();
    view->setLayoutDirection(Qt::LeftToRight);
    view = ui->apptheme_combobox->view();
    view->setLayoutDirection(Qt::LeftToRight);

}

void QSettingsPage::performConnections()
{
    connect(
         ui->codelanguage_combobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { m_codeEditor->setPlainText(m_codeSamples[index].second); }
    );

    connect(
         ui->codelanguage_combobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { m_codeEditor->setHighlighter(m_highlighters[index].second); }
    );

    connect(
        themefile_combobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { m_codeEditor->setCompleter(m_completers[index].second); }
    );

    connect(
       themefile_combobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { m_codeEditor->setSyntaxStyle(m_styles[index].second); }
    );

    connect(
        m_readOnlyCheckBox,
        &QCheckBox::stateChanged,
        [this](int state)
        { m_codeEditor->setReadOnly(state != 0); }
    );

    connect(
        m_wordWrapCheckBox,
        &QCheckBox::stateChanged,
        [this](int state)
        {
            if (state != 0)
            {
                m_codeEditor->setWordWrapMode(QTextOption::WordWrap);
            }
            else
            {
                m_codeEditor->setWordWrapMode(QTextOption::NoWrap);
            }
        }
    );

    connect(
        m_parenthesesEnabledCheckbox,
        &QCheckBox::stateChanged,
        [this](int state)
        { m_codeEditor->setAutoParentheses(state != 0); }
    );

    connect(
        m_tabReplaceEnabledCheckbox,
        &QCheckBox::stateChanged,
        [this](int state)
        { m_codeEditor->setTabReplace(state != 0); }
    );

    connect(
        m_tabReplaceNumberSpinbox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        [this](int value)
        { m_codeEditor->setTabReplaceSize(value); }
    );

    connect(
        m_autoIndentationCheckbox,
        &QCheckBox::stateChanged,
        [this](int state)
        { m_codeEditor->setAutoIndentation(state != 0); }
    );
    connect(ui->btn_reset, SIGNAL(clicked(bool)), this, SLOT(resetall(bool)));
}
void QSettingsPage::getSettingsValues(){
    heading = ui->heading_lineEdit->text();
    fontsize = (ui->fontsize_lineedit->text()).toInt();

    currentfont = (ui->fontComboBox->currentFont());
    currentfont.setPixelSize(fontsize);

    parenthesesPos = ui->parenthesis_checkbox->checkState();
    tabvalue = (ui->tablength_switcher->text()).toInt();
    autoindetentionPos = ui->indetention_checkbox->checkState();
    btndonatePos = ui->donatebtn_checkbox->checkState();
    wordwrapPos = ui->wordwrap_checkbox->checkState();
    blueiconclosePos = ui->blueiconclose_checkbox->checkState();

    codetheme = themefile_combobox->currentIndex();

    linenumbers = ui->showlinenumbers_checkbox->checkState();



}
void QSettingsPage::resetall(bool clicked){

    ui->parenthesis_checkbox->setChecked(true);
    ui->tablength_switcher->setText("4");
    ui->donatebtn_checkbox->setChecked(true);
    ui->wordwrap_checkbox->setChecked(false);
    ui->fontComboBox->setCurrentFont(QFont("Rubik", 9));
    ui->fontsize_lineedit->setText("22");
    ui->heading_lineEdit->setText("WhyCode");
    ui->showlinenumbers_checkbox->setChecked(true);
    themefile_combobox->setCurrentIndex(0);
    getSettingsValues();

}

QSettingsPage::~QSettingsPage()
{
    delete ui;
}
