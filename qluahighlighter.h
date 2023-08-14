#pragma once

// QCodeEditor
#include "qstylesyntaxhighlighter.h" // Required for inheritance
#include "qhighlightrule.h"
#include "qhighlightblockrule.h"

// Qt
#include <QRegularExpression>
#include <QVector>
#include <QTextCharFormat>

class QSyntaxStyle;

/**
 * @brief Class, that describes C++ code
 * highlighter.
 */
class QLuaHighlighter : public QStyleSyntaxHighlighter
{
    Q_OBJECT
public:

    /**
     * @brief Constructor.
     * @param document Pointer to document.
     */
    explicit QLuaHighlighter(QTextDocument* document=nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    QVector<QHighlightRule> m_highlightRules;
    QVector<QHighlightBlockRule> m_highlightBlockRules;

    QRegularExpression m_requirePattern;
    QRegularExpression m_functionPattern;
    QRegularExpression m_defTypePattern;
};
