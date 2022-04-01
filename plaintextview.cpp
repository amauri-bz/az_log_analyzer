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

void plainTextView::enableToolBtn(bool val) {
    ui->toolButton->setVisible(val);
}

void plainTextView::execMatch() {
    QTextCursor cursor = plainText.textCursor();
    QTextBlockFormat fmt;
    bool apply = false;

    projectMgr::Instance()->ReadProjFile();
    match_lines_.clear();

    cursor.setPosition(0);
    while(!cursor.atEnd())
    {
        cursor.select(QTextCursor::LineUnderCursor);
        if (cursor.block().isValid()) {
            apply = false;
            for(QString key : projectMgr::Instance()->proj_matchs_.keys())
            {
                if(cursor.block().text().contains(key)) {
                    fmt.setBackground(QColor(projectMgr::Instance()->proj_matchs_.value(key)));
                    cursor.setBlockFormat(fmt);
                    match_lines_[key].append(cursor.block().blockNumber()+1);
                    apply = true;
                    break;
                }
            }
            if(apply == false) {
                fmt.setBackground(QColor(QColorConstants::White));
                cursor.setBlockFormat(fmt);
            }
        }
        cursor.movePosition(QTextCursor::Down);
    }
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
