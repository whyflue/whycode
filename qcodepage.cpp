#include "qcodepage.h"
#include "ui_qcodepage.h"
#include "qcodeeditor.h"
#include "qglslcompleter.h"
#include "qluacompleter.h"
#include "qpythoncompleter.h"
#include "qsyntaxstyle.h"
#include "qcxxhighlighter.h"
#include "qglslhighlighter.h"
#include "qxmlhighlighter.h"
#include "qjsonhighlighter.h"
#include "qluahighlighter.h"
#include "qpythonhighlighter.h"

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
#include <QFileDialog>
#include <QMessageBox>
#include <string>


QString Last_FileName;
QString Last_FileContent;

QCodePage::QCodePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QCodePage)
{
    ui->setupUi(this);
    initData();
    createWidgets();

    pathLine = ui->lineedit_path;
    btn_open = ui->btn_open;
    btn_run = ui->btn_run;
    frame_codeedit = ui->frame_codeedit;
    frame_path = ui->frame_path;

    codechildren = nullptr;
    //codechildren->append(pathLine);
    //codechildren->append(btn_run);
    //codechildren->append(btn_open);
    //codechildren->append(frame_codeedit);
    //codechildren->append(frame_path);

    useless_layout = ui->useless_layout;
    //useless_layout.
}

void QCodePage::initData()
{

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
        {"Default", QSyntaxStyle::defaultStyle()},
        {"Psicho", QSyntaxStyle::psichoStyle()}
    };

    // Loading styles
    //loadStyle(":/resources/gui/themes/drakula.xml");
}
void QCodePage::createWidgets()
{

    //connect(ui->btn_open, SIGNAL(clicked()), this, SLOT(OpenFile()));
    connect(ui->btn_run, SIGNAL(clicked()), this, SLOT(SaveFile()));

    // CodeEditor
    m_codeEditor = new QCodeEditor(this);
    ui->box_codeedit->addWidget(m_codeEditor);

    int id = QFontDatabase::addApplicationFont(":/resources/gui/fonts/Rubik-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont rubik(family);
    rubik.setPointSize(22);
    m_codeEditor->setFont(QFont(rubik));

    m_codeEditor->setStyleSheet("QScrollBar:vertical {"
                                        "background: transparent;"         /*   убрать эту белую сетку в скроллбаре                 */
                                        "width: 14px;"
                                        "margin: 16px 2px 16px 2px;"
                                        "border-radius: 5px;"
                                    "}"


                                    "QScrollBar::handle:vertical {"
                                        "background-color: #4D545B;"
                                        "border: 1px solid #31363B;"
                                        "min-height: 8px;"
                                        "border-radius: 5px;"
                                    "}"

                                   " QScrollBar::handle:vertical:hover {"
                                        "background-color: #6A8AC8;"
                                        /*"border: 1px solid #6A8AC8;"*/
                                        "border-radius: 5px;"
                                       " min-height: 8px;"
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
                                    "}");

}
QCodePage::~QCodePage()
{
    delete ui;
}
void QCodePage::OpenFile()
{

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""));
         if(fileName == "") //If the user directly closes the file browsing dialog box, then the file name is empty and returns directly
    {
        return;
    }
    else
    {
       QFile file(fileName);
       if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
                       QMessageBox::warning(this,tr("error"),tr("open file failed"));
           return;
       }
       else
       {
           if(!file.isReadable())
           {
                               QMessageBox::warning(this,tr("error"),tr("the file is not readable"));
           }
           else
           {
                               QTextStream textStream(&file); //Read the file, use QTextStream
               while(!textStream.atEnd())
               {
                   m_codeEditor->setPlainText(textStream.readAll());
               }
               ui->lineedit_path->setText(fileName);
           }
       }
    }
}
 void QCodePage::CreateNewFile() //Create a new file
{
         m_codeEditor->clear(); //Clear the original file content
         bool Flag_IsNew = 1; //The new file flag is set to 1
         bool Flag_isOpen = 1; //New file creation flag bit is set to 1
}
void QCodePage::RunFile()
{
     SaveFile();

}
void QCodePage::SaveFile()
{
         if(Flag_IsNew) //If the new file flag bit is 1, the save file dialog box will pop up
    {
        if(m_codeEditor->toPlainText() == "")
        {
                         QMessageBox::warning(this,tr("Warning"),tr("The content cannot be empty!"),QMessageBox::Ok);
        }
        else
        {
            QFileDialog fileDialog;
            QString str = ui->lineedit_path->text();
            if(str == "")
            {
                return;
            }
            QFile filename(str);
            if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                                 QMessageBox::warning(this,tr("error"),tr("Failed to open file"),QMessageBox::Ok);
                return;
            }
            else
            {
                QTextStream textStream(&filename);
                QString str = m_codeEditor->toPlainText();
                textStream<<str;
                Last_FileContent = str;
            }
                         QMessageBox::information(this,"Save the file","Save the file successfully",QMessageBox::Ok);
            filename.close();
                         Flag_IsNew = 0; //The new file flag is recorded as 0
                         Last_FileName = str;//Save the content of the file
        }
    }
         else //Otherwise, the new file mark bit is 0, which means it is an old file, and the source file is directly saved and overwritten by default
    {
                 if(Flag_isOpen) //Determine whether a file is created or opened
        {
            QFile file(Last_FileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                                 QMessageBox::warning(this,tr("Warning"),tr("Failed to open file"));
                return;
            }
            else
            {
                QTextStream textStream(&file);
                QString str = m_codeEditor->toPlainText();
                textStream<<str;
                Last_FileContent = str;
                file.close();
            }
        }
        else
        {
                         QMessageBox::warning(this,tr("Warning"),tr("Please create or open the file first"));
            return;
        }
    }

}

void QCodePage::SaveFileAs() //Save as
{
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/home");
    if(fileName == "")
    {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
                 QMessageBox::warning(this,tr("error"),tr("open file failed"));
        return;
    }
    else
    {
        QTextStream textStream(&file);
        QString str = m_codeEditor->toPlainText();
        textStream<<str;
        QMessageBox::warning(this,tr("hint"),tr("save the file successfully"));
        Last_FileContent = str;
        Last_FileName = fileName;
        Flag_IsNew = 0;
        file.close();

    }
}


