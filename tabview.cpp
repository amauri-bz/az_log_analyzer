#include "tabview.h"
#include "ui_tabview.h"
#include "plaintextview.h"
#include <QFileInfo>
#include <QtDebug>

TabView::TabView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabView)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    this->setAcceptDrops(true);
}

TabView::~TabView()
{
    delete ui;
}

void TabView::newTab()
{
    ui->tabWidget->addTab(new plainTextView(), QString("Tab %0").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    emit s_set_status("Success operation", 2000);
}

void TabView::openFile(QString file_path)
{
    auto text = new plainTextView();
    QFileInfo fi(file_path);

    for(int i = 0; i  < ui->tabWidget->count(); i++) {
        if(ui->tabWidget->tabText(i) == fi.fileName()) {
            ui->tabWidget->setCurrentIndex(i);
            emit s_set_status("Success operation", 2000);
            return;
        }
    }

    if(text && text->openFile(file_path)) {
        ui->tabWidget->addTab(text, QString(fi.fileName()).arg(ui->tabWidget->count()+1));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        ui->tabWidget->setTabToolTip(ui->tabWidget->currentIndex(), fi.filePath());

        emit s_update_tree(text->match_lines());
        emit s_set_status("Success operation", 2000);
    }
    else
        emit s_set_status("Not opened", 2000);
}

void TabView::saveFile()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        QFileInfo fi(text->saveFile());
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        ui->tabWidget->setTabToolTip(ui->tabWidget->currentIndex(), fi.filePath());

        emit s_update_tree(text->match_lines());
        emit s_set_status("Success operation", 2000);
    }
    else
        emit s_set_status("Not saved", 2000);
}

void TabView::saveFileAs()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        QFileInfo fi(text->saveFileAs());
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        ui->tabWidget->setTabToolTip(ui->tabWidget->currentIndex(), fi.filePath());

        emit s_update_tree(text->match_lines());
        emit s_set_status("Success operation", 2000);
    }
    else
        emit s_set_status("Not saved", 2000);
}

void TabView::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);

    if(!ui->tabWidget->count()) {
        QMap<QString, QList<int>> empty;
        emit s_update_tree(empty);
    }
}

void TabView::reloadTabs()
{
    for(int i = 0; i  < ui->tabWidget->count(); i++) {
        if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->widget(i))) {
            text->execMatch();

            if(ui->tabWidget->widget(i)->isVisible())
                emit s_update_tree(text->match_lines());
        }
    }
}

void TabView::goToLine(int line)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->goToLine(line);
        emit s_set_status("Success operation", 2000);
    }
    else
        emit s_set_status("Line not found", 2000);
}

void TabView::findText(QString text, bool regex, bool whole_word, bool case_sensitive)
{
    if(plainTextView *text_edit = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        if(text_edit->findText(text, regex, whole_word, case_sensitive)) {
            emit s_set_status("Success operation", 2000);
        }
        else {
            emit s_set_status("Not found", 2000);
        }
    }
}

void TabView::on_tabWidget_currentChanged(int index)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        emit s_update_tree(text->match_lines());
    }
}

void TabView::zoomIn() {
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->zoomIn();
    }
}

void TabView::zoomOut() {
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->zoomOut();
    }
}

void TabView::addBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->addBookmark();
        emit s_update_tree(text->match_lines());
    }
}

void TabView::delBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->delBookmark();
        emit s_update_tree(text->match_lines());
    }
}

void TabView::cleanBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->cleanBookmark();
        emit s_update_tree(text->match_lines());
    }
}
