#include "myplaintextedit.h"
#include "linenumberarea.h"

#include <QPainter>
#include <QTextBlock>

/*!
    \brief Object constructor
*/
MyPlainTextEdit::MyPlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &MyPlainTextEdit::blockCountChanged, this, &MyPlainTextEdit::updateLineNumberAreaWidth);
    connect(this, &MyPlainTextEdit::updateRequest, this, &MyPlainTextEdit::updateLineNumberArea);
    connect(this, &MyPlainTextEdit::cursorPositionChanged, this, &MyPlainTextEdit::highlightCurrentLine);

    this->updateLineNumberAreaWidth(0);
    this->highlightCurrentLine();
    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    QFont font;
    font.setFamily("Courier New");
    font.setPointSize(10);
    this->setFont(font);
}

/*!
    \brief Calculate the line number area width
*/
int MyPlainTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

/*!
    \brief Update line number area width
*/
void MyPlainTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/*!
    \brief Update line number area
*/
void MyPlainTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

/*!
    \brief Handle resize event
*/
void MyPlainTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/*!
    \brief Highlight the actial line
*/
void MyPlainTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor("#eee");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

/*!
    \brief Handle paint area event
*/
void MyPlainTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
