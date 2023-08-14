QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcodeeditor.cpp \
    qcodepage.cpp \
    qcxxhighlighter.cpp \
    qexplorer.cpp \
    qframedtextattribute.cpp \
    qglslcompleter.cpp \
    qglslhighlighter.cpp \
    qhighlightblockrule.cpp \
    qjsonhighlighter.cpp \
    qlanguage.cpp \
    qlinenumberarea.cpp \
    qluacompleter.cpp \
    qluahighlighter.cpp \
    qpagebutton.cpp \
    qpythoncompleter.cpp \
    qpythonhighlighter.cpp \
    qsettingspage.cpp \
    qsidemenuswitcher.cpp \
    qstylesyntaxhighlighter.cpp \
    qsyntaxstyle.cpp \
    qtitletext.cpp \
    qxmlhighlighter.cpp \
    testbutton.cpp

HEADERS += \
    mainwindow.h \
    qcodeeditor.h \
    qcodepage.h \
    qcxxhighlighter.h \
    qexplorer.h \
    qframedtextattribute.h \
    qglslcompleter.h \
    qglslhighlighter.h \
    qhighlightblockrule.h \
    qhighlightrule.h \
    qjsonhighlighter.h \
    qlanguage.h \
    qlinenumberarea.h \
    qluacompleter.h \
    qluahighlighter.h \
    qpagebutton.h \
    qpythoncompleter.h \
    qpythonhighlighter.h \
    qsettingspage.h \
    qsidemenuswitcher.h \
    qstylesyntaxhighlighter.h \
    qsyntaxstyle.h \
    qtitletext.h \
    qxmlhighlighter.h \
    testbutton.h

FORMS += \
    mainwindow.ui \
    qcodepage.ui \
    qsettingspage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qcodeeditor_resources.qrc \
    resource.qrc
