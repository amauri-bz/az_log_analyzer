#ifndef PLAINTEXTVIEW_H
#define PLAINTEXTVIEW_H

#include <QFile>
#include <QMap>
#include <QWidget>
#include "myplaintextedit.h"

namespace Ui {
class plainTextView;
}

class plainTextView : public QWidget
{
    Q_OBJECT

public:
    explicit plainTextView(QWidget *parent = nullptr);
    ~plainTextView();
    bool openFile(QString file_path);
    QString saveFile();
    QString saveFileAs();
    void execMatch();
    void enableToolBtn(bool val);
    const QMap<QString, QList<int> > &match_lines() const;
    void goToLine(int line);
    bool findText(QString text, bool regex, bool whole_word, bool case_sensitive, bool up, bool down, bool arround);
    void zoomIn();
    void zoomOut();
    void addBookmark();
    void cleanBookmark();
    void delBookmark();
    const QString &getFile_path() const;

private slots:
    void on_toolButton_clicked();

private:
    bool fileParser(QString &file_path_);
    void fileFormatter();

    QString file_path;
    QMap<int, QColor> match_color_;
    QMap<QString, QList<int>> match_lines_;
    Ui::plainTextView *ui;
    MyPlainTextEdit plainText;
};

#endif // PLAINTEXTVIEW_H
