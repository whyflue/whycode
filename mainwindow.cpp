#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcodeeditor.h"
//#include "qglslcompleter.h"
//#include "qluacompleter.h"
//#include "qpythoncompleter.h"
#include "qsyntaxstyle.h"
#include "qcxxhighlighter.h"
#include "qglslhighlighter.h"
#include "qxmlhighlighter.h"
#include "qjsonhighlighter.h"
#include "qluahighlighter.h"
#include "qpythonhighlighter.h"
#include "qtitletext.h"
#include "qpagebutton.h"
#include "qsidemenuswitcher.h"
#include "qcodepage.h"
#include "qsettingspage.h"
#include "qexplorer.h"

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
#include <QPropertyAnimation>
#include <QFileSystemModel>
#include <QTreeView>
#include <QFontDatabase>
#include <QMenu>
#include <QPoint>
#include <QFileDialog>
#include <QXmlStreamReader>

bool fileChoosen = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_highlighterCombobox(nullptr),
      m_completerCombobox(nullptr),
      m_styleCombobox(nullptr),
      m_readOnlyCheckBox(nullptr),
      m_wordWrapCheckBox(nullptr),
      m_parenthesesEnabledCheckbox(nullptr),
      m_tabReplaceEnabledCheckbox(nullptr),
      m_tabReplaceNumberSpinbox(nullptr),
      m_autoIndentationCheckbox(nullptr),
      //m_codeEditor(nullptr),
      m_completers(),
      m_highlighters(),
      m_styles(),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_close->setIcon(QIcon(":/resources/gui/icons/icon_close.svg"));
    ui->btn_deploy->setIcon(QIcon(":/resources/gui/icons/icon_deploy.svg"));
    ui->btn_minimise->setIcon(QIcon(":/resources/gui/icons/icon_minimise.svg"));
    ui->btn_donate->setIcon(QIcon(":/resources/gui/icons/icon_donate.svg"));
    ui->btn_donate->setIconSize(QSize(19, 19));

    //codepage = new QCodePage();
    //ui->pages->addWidget(codepage);
    //ui->pages->setCurrentWidget(codepage);

    initData();
    createWidgets();
    setupWidgets();
    performConnections();
    menuswitcher->sidemenuOFF->click();
    applysettings();


    //initTitleBar();
}
void MainWindow::initData()
{

    m_highlighters = {
        {"None", nullptr},
        {"C++",  new QCXXHighlighter},
        {"GLSL", new QGLSLHighlighter},
        {"XML",  new QXMLHighlighter},
        {"JSON", new QJSONHighlighter},
        {"LUA",  new QLuaHighlighter},
        {"Python",  new QPythonHighlighter},
    };

    m_styles = {
        {"Default", QSyntaxStyle::defaultStyle()}
    };
    m_lightstyles = {
        {"defaultlight.xml", QSyntaxStyle::defaultLight()}
    };

    // Loading styles
    //loadStyle(":/resources/gui/themes/drakula.xml");
}

QString MainWindow::loadCode(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return QString();
    }

    return fl.readAll();
}

void MainWindow::loadStyle(QString path)
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

    m_lightstyles.append({style->name(), style});
}

void MainWindow::createWidgets()
{
    //Font
    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(16);

    //CodePage
    codepage = new QCodePage();
    codepage->setSizePolicy(QSizePolicy::Preferred, codepage->sizePolicy().verticalPolicy());
    codepage->setSizePolicy(QSizePolicy::Preferred, codepage->sizePolicy().horizontalPolicy());
    ui->pages->addWidget(codepage);
    ui->pages->setCurrentWidget(codepage);

    //SettingsPage
    settingspage = new QSettingsPage();
    settingspage->setSizePolicy(QSizePolicy::Preferred, settingspage->sizePolicy().verticalPolicy());
    settingspage->setSizePolicy(QSizePolicy::Preferred, settingspage->sizePolicy().horizontalPolicy());
    ui->pages->addWidget(settingspage);

    m_codeSampleCombobox  = new QComboBox();
    m_highlighterCombobox = new QComboBox();
    m_completerCombobox   = new QComboBox();

    // TitleText
    title = new QTitleText(this);
    title->setStyleSheet("QLabel { color: rgb(164, 164, 164); }");
    title->setFixedHeight(36);
    title->setSizePolicy(QSizePolicy::Preferred, title->sizePolicy().horizontalPolicy());
    title->clicked = false;


    // MenuButtons
    settingsbutton = new QPageButton(this);
    settingsbutton->setButtonText("Settings", rubik);
    settingsbutton->setButtonIcon(":/resources/gui/icons/icon_settings.svg");
    settingsbutton->setSizePolicy(QSizePolicy::Preferred, settingsbutton->sizePolicy().horizontalPolicy());
    connect(settingsbutton->text_button, SIGNAL(clicked(bool)), this, SLOT(settingsbutton_clicked()));
    connect(settingsbutton->icon_button, SIGNAL(clicked(bool)), this, SLOT(settingsbutton_clicked()));

    codepagebutton = new QPageButton(this);
    codepagebutton->setButtonText("cpb", rubik);
    codepagebutton->setButtonIcon(":/resources/gui/icons/icon_open_selected.svg");
    codepagebutton->setSizePolicy(QSizePolicy::Preferred, settingsbutton->sizePolicy().horizontalPolicy());
    connect(codepagebutton->text_button, SIGNAL(clicked(bool)), this, SLOT(codepagebutton_clicked()));
    connect(codepagebutton->icon_button, SIGNAL(clicked(bool)), this, SLOT(codepagebutton_clicked()));

    whycombutton = new QPageButton(this);
    whycombutton->setButtonText("WhyCompany", QFont("Typold", 13));
    whycombutton->setButtonIcon(":/resources/gui/icons/icon_wclogo.svg");
    whycombutton->setSizePolicy(QSizePolicy::Preferred, settingsbutton->sizePolicy().horizontalPolicy());
    connect(whycombutton->text_button, SIGNAL(clicked(bool)), this, SLOT(whycombutton_clicked()));
    connect(whycombutton->icon_button, SIGNAL(clicked(bool)), this, SLOT(whycombutton_clicked()));


    //MenySwitcher
    menuswitcher = new QSideMenuSwitcher(this);
    connect(menuswitcher->sidemenuOFF, SIGNAL(clicked(bool)), this, SLOT(leftMenuClose(bool)));
    connect(menuswitcher->sidemenuON, SIGNAL(clicked(bool)), this, SLOT(leftMenuClose(bool)));

    //Explorer

    codepagebutton2 = new QPageButton(this);
    codepagebutton2->setButtonIcon(":/resources/gui/icons/icon_open_selected.svg");
    codepagebutton2->setButtonText("Open", rubik);
    codepagebutton2->setSizePolicy(QSizePolicy::Preferred, settingsbutton->sizePolicy().horizontalPolicy());
    connect(codepagebutton2->icon_button, SIGNAL(clicked(bool)), this, SLOT(codepagebutton_clicked()));
    connect(codepagebutton2->text_button, SIGNAL(clicked(bool)), this, SLOT(codepagebutton_clicked()));
    connect(codepagebutton2->icon_button, SIGNAL(clicked(bool)), this, SLOT(OpenFile()));

    fileexplorer = new QExplorer(this);
    fileexplorer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    fileexplorer->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fileexplorer->treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotTreeMenu(const QPoint&)));
    connect(fileexplorer->treeView, SIGNAL(doubleClicked(const QModelIndex& )), this, SLOT(on_fileexplorerDoubleClicked(const QModelIndex&)));

    //connect(codepagebutton->text_button, SIGNAL(doubleClicked()), this, SLOT(filexplorer->OpenFile()));

    codeexplorer_bg = new QFrame(this);
    codeexplorer_layout = new QVBoxLayout(codeexplorer_bg);
    codeexplorer_bg->setStyleSheet("border-radius: 5px;"
                                  "background-color: #272727;");
    codeexplorer_bg->setMaximumHeight(250);
    codeexplorer_layout->addWidget(codepagebutton);
    codeexplorer_layout->addWidget(fileexplorer);
    codeexplorer_layout->setSpacing(6);
    codeexplorer_layout->setContentsMargins(0, 0, 6, 6); // left, top, right, bottom
    //codeexplorer_layout->addSpacerItem(new QSpacerItem(1, -5, QSizePolicy::Minimum, QSizePolicy::Fixed));
    codeexplorer_bg->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Adding widgets
    ui->box_leftmenu->addWidget(whycombutton);
    /*ui->box_leftmenu->addWidget(new QLabel(tr("Code sample"), this));
    ui->box_leftmenu->addWidget(m_codeSampleCombobox);
    ui->box_leftmenu->addWidget(new QLabel(tr("Completer"), this));
    ui->box_leftmenu->addWidget(m_completerCombobox);
    ui->box_leftmenu->addWidget(new QLabel(tr("Highlighter"), this));
    ui->box_leftmenu->addWidget(m_highlighterCombobox);*/
    ui->box_leftmenu->addWidget(codepagebutton2);
    //codepagebutton2->hide();
    ui->box_leftmenu->addWidget(codeexplorer_bg);
    codeexplorer_bg->hide();
    ui->box_leftmenu->addSpacerItem(new QSpacerItem(1, 2, QSizePolicy::Minimum, QSizePolicy::Expanding));
    ui->box_leftmenu->addWidget(settingsbutton);


    ui->title_layout->addWidget(title, 0, 2, Qt::Alignment(Qt::AlignCenter));

    QHBoxLayout switcher_layout = QHBoxLayout(ui->switcher_frame);
    switcher_layout.addWidget(menuswitcher);
}
void MainWindow::setupWidgets()
{

    // CodeEditor
    //m_codeEditor->setPlainText  (m_codeSamples[0].second);
    codepage->m_codeEditor->setSyntaxStyle(codepage->m_styles[0].second);
    codepage->m_codeEditor->setCompleter  (codepage->m_completers[0].second);
    codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[0].second);

    QStringList list;
    // Code samples
    for (auto&& el : codepage->m_codeSamples)
    {
        list << el.first;
    }

    m_codeSampleCombobox->addItems(list);
    list.clear();

    // Highlighter
    for (auto&& el : codepage->m_highlighters)
    {
        list << el.first;
    }

    m_highlighterCombobox->addItems(list);
    list.clear();

    // Completer
    for (auto&& el : codepage->m_completers)
    {
        list << el.first;
    }

    m_completerCombobox->addItems(list);
    list.clear();

}
void MainWindow::performConnections()
{
    connect(
        m_codeSampleCombobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { codepage->m_codeEditor->setPlainText(codepage->m_codeSamples[index].second); }
    );

    connect(
        m_highlighterCombobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[index].second); }
    );

    connect(
        m_completerCombobox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        { codepage->m_codeEditor->setCompleter(codepage->m_completers[index].second); }
    );

    connect(settingspage->applySettingsButton, SIGNAL(clicked()), this, SLOT(applysettings()));
}

void MainWindow::settingsbutton_clicked()
{
    this->showMaximized();
    title->clicked = true;
    ui->pages->setCurrentWidget(settingspage);
    if(fileChoosen == false)
    {
        codepagebutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_open.svg"));
        codepagebutton2->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_open.svg"));
    }
    else
    {
        codepagebutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_file.svg"));
        codepagebutton2->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_file.svg"));
    }
    settingsbutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_settings_selected"));
}
void MainWindow::codepagebutton_clicked()
{
    ui->pages->setCurrentWidget(codepage);
    if(fileChoosen == false)
    {
        codepagebutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_open_selected"));
        codepagebutton2->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_open_selected"));
    }
    else
    {
        codepagebutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_code_selected"));
        codepagebutton2->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_code_selected"));
    }
    settingsbutton->icon_button->setIcon(QIcon(":/resources/gui/icons/icon_settings"));

}
void MainWindow::whycombutton_clicked()
{
    QDesktopServices :: openUrl( QUrl ("https://whycompany.github.io/WhyCode/"));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btn_donate_clicked()
{
    QDesktopServices :: openUrl( QUrl ("https://www.donationalerts.com/r/whycompany"));
}


void MainWindow::on_btn_close_clicked()
{
    this ->close();
}
void MainWindow::on_btn_deploy_clicked()
{
    if(title->clicked == false)
    {
        this->showMaximized();
        title->clicked = true;
    }
    else
    {
        this->showNormal();
        title->clicked = false;
        codepagebutton_clicked();
    }
}
void MainWindow::on_btn_minimise_clicked()
{
    this->showMinimized();
}
void MainWindow::applysettings(){
    settingspage->getSettingsValues();

    title->title_text->setText(settingspage->heading);

    codepage->m_codeEditor->setFont(QFont(settingspage->currentfont));

    if (settingspage->wordwrapPos == true)
    {
        codepage->m_codeEditor->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        codepage->m_codeEditor->setWordWrapMode(QTextOption::NoWrap);
    };

    codepage->m_codeEditor->setTabReplaceSize(settingspage->tabvalue);

    codepage->m_codeEditor->setAutoParentheses(settingspage->parenthesesPos);

    if(settingspage->btndonatePos == true){
        ui->btn_donate->show();
    }
    else{
        ui->btn_donate->hide();
    }
    if(settingspage->blueiconclosePos == true){
        ui->btn_close->setStyleSheet("QPushButton:hover{icon: url(\":/resources/gui/icons/icon_close_hovered_whitemode.svg\");}");
    }
    else{
       ui->btn_close->setStyleSheet("QPushButton:hover{icon: url(\":/resources/gui/icons/icon_close_hovered.svg\");}");
    }
    codepage->m_codeEditor->setSyntaxStyle(codepage->m_styles[settingspage->codetheme].second);
    settingspage->m_codeEditor->setFont(QFont(settingspage->currentfont));

    QFile file(":/resources/whylightmode.qss");
    if(file.open(QFile::ReadOnly)) {
       QString StyleSheet = QLatin1String(file.readAll());
       file.close();
       QObjectList children = ui->centralwidget->children();
       children.append(ui->btn_donate);
       children.append(ui->frame_2);
       ui->centralwidget->setStyleSheet("background-color: #D9D9D9;");
       foreach(QObject * child, children) {
            if (child->isWidgetType()) {
                ((QWidget *)child)->setStyleSheet(StyleSheet);
            }
       }
       //children.clear();
       //children.append(codepage->codechildren[0]);
       children = codepage->useless_layout->children();
       children.append(codepage->pathLine);
       children.append(codepage->m_codeEditor);
       foreach(QObject * child, children) {
            if (child->isWidgetType())
            {
                ((QWidget *)child)->setStyleSheet(StyleSheet);
            }
       }
       int index = settingspage->themefile_combobox->currentIndex();
       settingspage->themefile_combobox->disconnect();
       settingspage->themefile_combobox->clear();

       loadStyle(":recources/gui/themes/default_whylightmode.xml");

       QStringList list;
       for (auto&& el : m_lightstyles)
       {
           list << el.first;
       }
       settingspage->themefile_combobox->addItems(list);
       //settingspage->themefile_combobox->disconnect();
       connect(
          settingspage->themefile_combobox,
           QOverload<int>::of(&QComboBox::currentIndexChanged),
           [this](int index)
           { settingspage->m_codeEditor->setSyntaxStyle(m_lightstyles[index].second); }
       );
       /*connect(
          settingspage->themefile_combobox,
           QOverload<int>::of(&QComboBox::currentIndexChanged),
           [this](int index)
           { codepage->m_codeEditor->setSyntaxStyle(m_lightstyles[index].second); }
       );*/
       //settingspage->themefile_combobox->addItems(list);
       codepage->m_codeEditor->setSyntaxStyle(m_lightstyles[index].second);
       settingspage->m_codeEditor->setSyntaxStyle((m_lightstyles[index].second));
       codepage->m_codeEditor->setStyleSheet("color: #393939;"
                                             "background-color: #AFAFAF");
       settingspage->m_codeEditor->setStyleSheet("color: #393939;"
                                               "background-color: #AFAFAF;");
       settingspage->themefile_combobox->setCurrentIndex(index);

       children = settingspage->settings_bg->children();
       children.append(settingspage->settings_bg);
       foreach(QObject * child, children) {
            if (child->isWidgetType())
            {
                ((QWidget *)child)->setStyleSheet(StyleSheet);
            }
       }

       children.append(settingsbutton);
       children.append(whycombutton);
       children.append(codepagebutton);
       children.append(codepagebutton2);
       children.append(menuswitcher);
       foreach(QObject * child, children) {
            if (child->isWidgetType())
            {
                ((QWidget *)child)->setStyleSheet(StyleSheet);
            }
       }
       //QColor bgcolor = QWidget::palette().color(settingsbutton->m_button_bg->backgroundRole());
       QWidget::palette().color(settingsbutton->m_button_bg->backgroundRole()).isValid();

       file.close();
    }
}
void MainWindow::leftMenuClose(bool menu_pressed){
        QPropertyAnimation *animation = new QPropertyAnimation(ui->leftmenu_frame, "size", this);
        menu_pressed = menuswitcher->menuPosition;
        animation->setDuration(300);
        animation->setEasingCurve(QEasingCurve::InCurve);

        if(menu_pressed == true)
            {
                //ui->leftmenu_frame->setSizePolicy(QSizePolicy::Preferred, ui->leftmenu_frame->sizePolicy().horizontalPolicy());
                animation->setStartValue(QSize(202, ui->frame_titlebar->height() + 6 + ui->pages->height()));
                animation->setEndValue(QSize(51, ui->frame_titlebar->height() + 6 + ui->pages->height()));
                menuswitcher->menuPosition = false;
                animation->start();
                ui->leftmenu_frame->setFixedWidth(51);
                /*connect(animation, &QPropertyAnimation::finished, codepagebutton->text_button, &QPushButton::hide );
                connect(animation, &QPropertyAnimation::finished, settingsbutton->text_button, &QPushButton::hide );
                connect(animation, &QPropertyAnimation::finished, whycombutton->text_button, &QPushButton::hide );
                connect(animation, &QPropertyAnimation::finished, whycombutton->text_button, &QPushButton::hide );*/
                menuswitcher->sidemenuON->setIcon(QIcon(":/resources/gui/icons/menu_off_selection_btn_selected.svg"));
                menuswitcher->sidemenuOFF->setIcon(QIcon(":/resources/gui/icons/menu_on_selection_btn.svg"));


                whycombutton->text_button->hide();

                if(fileChoosen == true)
                {
                    codepagebutton2->show();
                    codeexplorer_bg->hide();
                }
                codepagebutton2->text_button->hide();

                settingsbutton->text_button->hide();
                whycombutton->m_button_bg->setStyleSheet("background-color: none;");
                codepagebutton2->m_button_bg->setStyleSheet("background-color: none;");
                settingsbutton->m_button_bg->setStyleSheet("background-color: none;");

                //QColor bgcolor = QWidget::palette().color(settingsbutton->m_button_bg->backgroundRole());


             }
        else{
            //ui->leftmenu_frame->setSizePolicy(QSizePolicy::Preferred, ui->leftmenu_frame->sizePolicy().horizontalPolicy());
            if(QWidget::palette().color(settingsbutton->m_button_bg->backgroundRole()).isValid())
            {
                QColor bgcolor = QWidget::palette().color(settingsbutton->m_button_bg->backgroundRole()).isValid();
            }
            if(fileChoosen == true)
            {
                codepagebutton2->hide();
                codeexplorer_bg->show();
            }
            else if(fileChoosen == false)
            {
                codepagebutton2->text_button->show();
                codepagebutton2->m_button_bg->setStyleSheet("background-color: #272727;");

            }

            settingsbutton->text_button->show();
            whycombutton->text_button->show();
            //settingsbutton->setBackgroundRole(bgcolor);
            codepagebutton->m_button_bg->setStyleSheet("background-color: #272727;");
            settingsbutton->m_button_bg->setStyleSheet("background-color: #272727;");
            whycombutton->m_button_bg->setStyleSheet("background-color: #272727;");

            animation->setStartValue(QSize(ui->leftmenu_frame->width(), ui->frame_titlebar->height() + 6 + ui->pages->height()));
            animation->setEndValue(QSize(202, ui->frame_titlebar->height() + 6 + ui->pages->height()));
            menuswitcher->sidemenuON->setIcon(QIcon(":/resources/gui/icons/menu_off_selection_btn.svg"));
            menuswitcher->sidemenuOFF->setIcon(QIcon(":/resources/gui/icons/menu_on_selection_btn_selected.svg"));
            menuswitcher->menuPosition = true;
            animation->start();


            ui->leftmenu_frame->setFixedWidth(202);
            //ui->leftmenu_frame->setSizePolicy(QSizePolicy::Fixed, ui->leftmenu_frame->sizePolicy().horizontalPolicy());
        }
}
void MainWindow::on_fileexplorerDoubleClicked(const QModelIndex &index)
{
    //Font
    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(16);

    QFileInfo fileinfo(fileexplorer->dirmodel->filePath(index));
    QFile file(fileexplorer->dirmodel->filePath(index));
    QString suf = fileinfo.suffix();
    QString filePath, fileName;
    filePath = fileexplorer->dirmodel->filePath(index);
    fileName = fileinfo.fileName();

    //codepage->pathLine->setText(fileName);


    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString text = in.readAll();
        codepage->m_codeEditor->setPlainText(text);
        codepage->pathLine->setText(filePath);
        codepagebutton->setButtonText(fileName, rubik);
        codepagebutton_clicked();
        file.close();
    }
    if(suf == "json")
    {
        codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[3].second);
    }
    else if(suf == "cpp" || suf == "hpp" || suf == "h")
    {
        codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[0].second);
    }
    else if(suf == "xml" || suf == "ui")
    {
        codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[2].second);
    }
    else if(suf == "glsl")
    {
        codepage->m_codeEditor->setHighlighter(codepage->m_highlighters[1].second);
    }
}
void MainWindow::slotTreeMenu(const QPoint &point) //Slot function definition
{
       QMenu *menu = new QMenu(this);
                QAction *a1=new QAction(tr("Upload"));
       menu->addAction(a1);
                QAction *a2=new QAction(tr("Mobile"));
       menu->addAction(a2);
                QAction *a3=new QAction(tr("copy"));
       menu->addAction(a3);
                QAction *a4=new QAction(tr("Delete"));
       menu->addAction(a4);
       //QModelIndex curIndex = fileexplorer->treeView->indexAt(point); //The index of the currently clicked element
       //QModelIndex index = curIndex.sibling(curIndex.row(),0);
       menu->exec(QCursor::pos());
       //menu->exec(fileexplorer->treeView->viewport()->mapToGlobal(point));

}
void MainWindow::OpenFile()
{
    bool menu_pressed;
    menu_pressed = menuswitcher->menuPosition;

    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(16);

    /*codeexplorer_bg->show();
    codepagebutton2->hide();
    fileexplorer->show();*/

    QString folderName;
    folderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "/home",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    if(!folderName.isNull()){
        if(menu_pressed == true)
        {
            codeexplorer_bg->show();
            codepagebutton2->hide();
        }
        //fileexplorer->show();
        fileexplorer->dirmodel->setRootPath(folderName);
        fileexplorer->treeView->setRootIndex(fileexplorer->dirmodel->index(folderName));
        codepagebutton->setButtonText("Code", rubik);
        fileChoosen = true;
        codepagebutton_clicked();
        codepagebutton2->icon_button->disconnect();
        connect(codepagebutton2->icon_button, SIGNAL(clicked(bool)), this, SLOT(codepagebutton_clicked()));

    }
}



