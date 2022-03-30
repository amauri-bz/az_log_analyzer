#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "plaintextview.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(MyPlainTextEdit *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
       {
           return QSize(codeEditor->lineNumberAreaWidth(), 0);
       }

   protected:
       void paintEvent(QPaintEvent *event) override
       {
           codeEditor->lineNumberAreaPaintEvent(event);
       }

   private:
       MyPlainTextEdit *codeEditor;

};

#endif // LINENUMBERAREA_H
