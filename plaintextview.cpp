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

plainTextView::~plainTextView()
{
    delete ui;
}

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

void plainTextView::fileParser(QString &file_path_) {
    QFile file(file_path_);
    projectMgr::Instance()->ReadProjFile();

    if(!file.exists()) {
        return;
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
}

void plainTextView::enableToolBtn(bool val) {
    ui->toolButton->setVisible(val);
}

void plainTextView::execMatch() {
    match_lines_.clear();
    match_color_.clear();

    fileParser(file_path);
    fileFormatter();
}

bool plainTextView::setFile(QString file_path_) {
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

QString plainTextView::saveFileAs() {
    this->file_path.clear();
    return this->saveFile();
}

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

void plainTextView::on_toolButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if(color.isValid()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->clear();
        clipboard->setText(color.name());
    }
}

const QMap<QString, QList<int>>& plainTextView::match_lines() const
{
    return match_lines_;
}

void plainTextView::goToLine(int line)
{
    QTextCursor cursor(plainText.document()->findBlockByLineNumber(line-1));
    plainText.setTextCursor(cursor);
}

bool plainTextView::findText(QString text, bool regex, bool whole_word, bool backward, bool case_sensitive)
{
    QTextDocument::FindFlags flag{0};

    if (backward)
        flag = flag | QTextDocument::FindBackward;
    if (case_sensitive)
        flag = flag | QTextDocument::FindCaseSensitively;
    if (whole_word)
        flag = flag | QTextDocument::FindWholeWords;

    if(regex) {
        return plainText.find(QRegularExpression(text), flag);
    }

    return plainText.find(text, flag);
}

void plainTextView::zoomIn() {
    plainText.zoomIn();
}

void plainTextView::zoomOut() {
    plainText.zoomOut();
}
