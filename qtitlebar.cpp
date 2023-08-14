/*#include "qtitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QFile>
#include <QMouseEvent>

#define  BUTTON_HEIGHT 30 // Button height;
#define  BUTTON_WIDTH 30 // Button width;
#define  TITLE_HEIGHT 30 // Title bar height;

QTitleBar::QTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_colorR(153)
    , m_colorG(153)
    , m_colorB(153)
    , m_isPressed(false)
    , m_buttonType(MIN_MAX_BUTTON)
    , m_windowBorderWidth(0)
    , m_isTransparent(false)
{
    // Initialization;
    initControl();
    initConnections();
    // Load the local style MyTitle.css file;
    //loadStyleSheet("MyTitle");
}

QTitleBar::~QTitleBar()
{

}

// Initialize the control;
void QTitleBar::initControl()
{
    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel("WhyCode");
    m_pTitleContent->setFont(QFont("Rubik", 20));

    m_pButtonMin = new QPushButton;
    m_pButtonRestore = new QPushButton;
    m_pButtonMax = new QPushButton;
    m_pButtonClose = new QPushButton;

    m_pButtonMin->setFixedSize(QSize(36, 36));
    m_pButtonRestore->setFixedSize(QSize(36, 36));
    m_pButtonMax->setFixedSize(QSize(36, 36));
    m_pButtonClose->setFixedSize(QSize(36, 36));

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");

    m_pButtonMin->setToolTip(QStringLiteral("minimize"));
    m_pButtonRestore->setToolTip(QStringLiteral("reduction"));
    m_pButtonMax->setToolTip(QStringLiteral("maximize"));
    m_pButtonClose->setToolTip(QStringLiteral("shut down"));

    QHBoxLayout* mylayout = new QHBoxLayout(this);
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestore);
    mylayout->addWidget(m_pButtonMax);
    mylayout->addWidget(m_pButtonClose);

    mylayout->setContentsMargins(5, 0, 0, 0);
    mylayout->setSpacing(0);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(36);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

// Binding of signal slots;
void QTitleBar::initConnections()
{
    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

// Set the background color of the title bar and draw the background color of the title bar in the paintEvent event;
/* The default value is given in the constructor, you can set the color value externally to change the background color of the title bar;
void QTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_isTransparent = isTransparent;
    // Repaint (call paintEvent event);
    update();
}*/

/* Set the title bar icon;
void QTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}*/

/* Set the title content;
void QTitleBar::setTitleContent(QString titleContent, int titleFontSize)
{
    // Set the title font size;
    QFont font = m_pTitleContent->font();
    font.setPointSize(titleFontSize);
    m_pTitleContent->setFont(font);
    // Set the title content;
    m_pTitleContent->setText(titleContent);
    m_titleContent = titleContent;
}

// Set the length of the title bar;
void QTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

// Set the button type on the title bar;
// Since the buttons on the title bar of different windows are different, you can customize the buttons in the title bar;
// Four buttons are provided here, which are minimize, restore, maximize, and close buttons. If you need other buttons, you can add settings by yourself;
void QTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType = buttonType;

    switch (buttonType)
    {
    case MIN_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    case MIN_MAX_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
        }
        break;
    case ONLY_CLOSE_BUTTON:
        {
            m_pButtonMin->setVisible(false);
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    default:
        break;
    }
}

// Set whether the title in the title bar will automatically scroll, the effect of the marquee;
// Under normal circumstances, the title content in the title bar is not scrollable, but since you customize it, it depends on your needs. You can do O(∩_∩)O if you want to design it!
void QTitleBar::setTitleRoll()
{
    connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

// Set the width of the window border;
void QTitleBar::setWindowBorderWidth(int borderWidth)
{
    m_windowBorderWidth = borderWidth;
}

// Save the position and size of the window before the window is maximized;
void QTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

// Get the position and size of the window before the window is maximized;
void QTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
    point = m_restorePos;
    size = m_restoreSize;
}

// Draw the background color of the title bar;
void QTitleBar::paintEvent(QPaintEvent *event)
{
    // Whether to set the title transparent;
    if (!m_isTransparent)
    {
        //Set the background color;
        QPainter painter(this);
        QPainterPath pathBack;
        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
    }

    // When the window is maximized or restored, the length of the window changes, and the length of the title bar should be changed together;
    // The m_windowBorderWidth is subtracted here because the window may have a border of different width;
    // If the window has a border, you need to set the value of m_windowBorderWidth, otherwise m_windowBorderWidth defaults to 0;
    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
    {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
    QWidget::paintEvent(event);
}

// Double-click to respond to events, mainly to achieve double-clicking the title bar to maximize and minimize operations;
void QTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Double-click is only valid when there are maximize and restore buttons;
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        // Determine whether the current window is maximized or original size by the state of the maximize button;
        // Or by setting variables separately to represent the current window state;
        if (m_pButtonMax->isVisible())
        {
            onButtonMaxClicked();
        }
        else
        {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}

// The following three events, mousePressEvent, mouseMoveEvent, and mouseReleaseEvent, realize the effect of dragging the title bar with the mouse to move the window;
void QTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        // Do not drag the window when the window is maximized;
        if (m_pButtonMax->isVisible())
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
    else
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void QTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void QTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

// Load local style files;
// You can write the style directly in the file, and load it directly when the program is running;
/*void QTitleBar::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/Resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

// The following is the slot for button operation response;
void QTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void QTitleBar::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void QTitleBar::onButtonMaxClicked()
{
    m_pButtonMax->setVisible(false);
    m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void QTitleBar::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}

// This method is mainly to display the title in the title bar as a scrolling effect;
void QTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;
    // When the intercepted position is longer than the string, start from the beginning;
    if (nPos > titleContent.length())
        nPos = 0;

    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}
*/
