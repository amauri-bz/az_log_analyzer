#include "plainTextView.h"
#include "ui_plainTextView.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSaveFile>
#include <QDebug>
#include <QTextBlock>
#include <QClipboard>
#include <QColorDialog>
#include <QRegularExpression>
#include <QPainter>
#include "projectmgr.h"


/*!
    \brief Object constructor
*/
plainTextView::plainTextView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plainTextView),
    file_path("")
{
  ui->setupUi(this);
  enableToolBtn(false);
  this->setAcceptDrops(true);
  this->layout()->addWidget(&plainText);
}

/*!
    \brief Object destructor
*/
plainTextView::~plainTextView()
{
    delete ui;
}

/*!
    \brief Apply colors in parsed the text file
*/
void plainTextView::fileFormatter() {
    QTextCursor cursor = plainText.textCursor();
    QTextBlockFormat fmt;

    cursor.select(QTextCursor::Document);
    fmt.setBackground(QColor(QColorConstants::White));
    cursor.setBlockFormat(fmt);

    cursor.movePosition(QTextCursor::Start);
    int i = plainText.document()->blockCount();
    while(i >= 0)
    {
        if(cursor.block().isValid() && match_color_.contains(cursor.block().blockNumber())) {
            cursor.select(QTextCursor::LineUnderCursor);
            fmt.setBackground(match_color_.value(cursor.block().blockNumber()));
            cursor.setBlockFormat(fmt);
        }
        cursor.movePosition(QTextCursor::Down);
        i--;
    }
}

/*!
    \brief File path getter
*/
const QString &plainTextView::getFile_path() const
{
    return file_path;
}

/*!
    \brief Parse the text file looking for patterns
*/
bool plainTextView::fileParser(QString &file_path_) {
    QFile file(file_path_);
    projectMgr::Instance()->ReadProjFile();

    if(!file.exists()) {
        return false;
    }

    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream file_stream(&file);

        file_stream.setAutoDetectUnicode(true);
        QString line = QString();
        int counter = 0;

        while(!file_stream.atEnd())
        {
            line = QString(file_stream.readLine());
            if (!line.isEmpty())
            {
                for(QString key : projectMgr::Instance()->proj_matchs_.keys())
                {
                    if(line.contains(key)) {
                        match_lines_[key].append(counter+1);
                        match_color_[counter] =
                                QColor(
                                    projectMgr::Instance()->proj_matchs_.value(key)
                                );
                        break;
                    }
                }
            }
            counter++;
        }
    }
    return true;
}

/*!
    \brief Enable color tool button for project tab
*/
void plainTextView::enableToolBtn(bool val) {
    ui->toolButton->setVisible(val);
}

/*!
    \brief Run parse and formatter in the text file
*/
void plainTextView::execMatch() {
    QList<int> bookMark;
    if(match_lines_.contains("BookMarks"))
        bookMark = match_lines_["BookMarks"];

    match_lines_.clear();
    match_color_.clear();

    if(fileParser(file_path)) {
        fileFormatter();
        if(bookMark.count() > 0) {
            match_lines_["BookMarks"] = bookMark;
            projectMgr::Instance()->proj_matchs_["BookMarks"] = "#fff";
        }
    }
}

/*!
    \brief Handle open file signal from main-windows and tree view
*/
bool plainTextView::openFile(QString file_path_) {
    QFile file(file_path_);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream file_stream(&file);
            plainText.setPlainText(file_stream.readAll());
            this->file_path = file_path_;
            execMatch();
            return true;
        }
    }
    return false;
}

/*!
    \brief Handle save-file-as signal from main-windows
*/
QString plainTextView::saveFileAs() {
    this->file_path.clear();
    return this->saveFile();
}

/*!
    \brief Handle save-file signal from main-windows
*/
QString plainTextView::saveFile() {
    QString file_name{};

    if(this->file_path.isEmpty()) {
        file_name = QFileDialog::getSaveFileName(this,
                tr("Save File"), this->file_path,
                tr("All Files (*);;*.json;;*.txt;;*.log"));
        this->file_path = file_name;
    }

    QSaveFile file(this->file_path);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << plainText.toPlainText();
        if (!file.commit()) {
          QMessageBox::information(this, tr("Unable to save file"),
              file.errorString());
        }
    } else {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
    }
    execMatch();
    return this->file_path;
}

/*!
    \brief Handle tool button click event
*/
void plainTextView::on_toolButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if(color.isValid()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->clear();
        clipboard->setText(color.name());
    }
}

/*!
    \brief Handle tool button click event
*/
const QMap<QString, QList<int>>& plainTextView::match_lines() const
{
    return match_lines_;
}

/*!
    \brief Handle got-to-line signal from index view
*/
void plainTextView::goToLine(int line)
{
    QTextCursor cursor(plainText.document()->findBlockByLineNumber(line-1));
    plainText.setTextCursor(cursor);
}

/*!
    \brief Handle find signal from find view
*/
bool plainTextView::findText(QString text, bool regex, bool whole_word, bool case_sensitive, bool up, bool down,bool arround)
{
    QTextDocument::FindFlags flag{0};

    if (case_sensitive)
        flag = flag | QTextDocument::FindCaseSensitively;
    if (whole_word)
        flag = flag | QTextDocument::FindWholeWords;

    if(regex) {
        if(up) {
            flag = flag | QTextDocument::FindBackward;
            return plainText.find(QRegularExpression(text), flag);
        }
        else if(down) {
            return plainText.find(QRegularExpression(text), flag);
        }

        if(!plainText.find(QRegularExpression(text), flag)) {
             flag = flag | QTextDocument::FindBackward;
             return plainText.find(QRegularExpression(text), flag);
        }
    }
    else {
        if(up) {
            flag = flag | QTextDocument::FindBackward;
            return plainText.find(text, flag);
        }
        else if(down) {
            return plainText.find(text, flag);
        }

        if(!plainText.find(text, flag)) {
            flag = flag | QTextDocument::FindBackward;
            return plainText.find(text, flag);
        }
    }
    return true;
}

/*!
    \brief Handle zoom signal from main-window
*/
void plainTextView::zoomIn() {
    plainText.zoomIn();
}

/*!
    \brief Handle zoom signal from main-window
*/
void plainTextView::zoomOut() {
    plainText.zoomOut();
}

/*!
    \brief Handle add bookmark signal from main-window
*/
void plainTextView::addBookmark() {
    QTextCursor cursor = plainText.textCursor();
    match_lines_["BookMarks"].append(cursor.block().blockNumber()+1);
    projectMgr::Instance()->proj_matchs_["BookMarks"] = "#fff";
}

/*!
    \brief Handle delete bookmark signal from main-window
*/
void plainTextView::delBookmark() {
    QTextCursor cursor = plainText.textCursor();
    match_lines_["BookMarks"].removeIf(
                [=](int i){ return i == (cursor.block().blockNumber()+1);}
    );
    if(!projectMgr::Instance()->proj_matchs_["BookMarks"].count())
        cleanBookmark();
}

/*!
    \brief Handle clear bookmark signal from main-window
*/
void plainTextView::cleanBookmark() {
    match_lines_.remove("BookMarks");
    projectMgr::Instance()->proj_matchs_.remove("BookMarks");
}
