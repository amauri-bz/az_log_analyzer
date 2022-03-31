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
}

void TabView::openFile(QString file_path)
{
    auto text = new plainTextView();
    if(text && text->setFile(file_path)) {
        QFileInfo fi(file_path);
        ui->tabWidget->addTab(text, QString(fi.fileName()).arg(ui->tabWidget->count()+1));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

        emit s_update_tree(text->match_lines());
    }
    else {
        qInfo() << "[ERROR] open tab and file\n";
    }
}

void TabView::saveFile()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        QFileInfo fi(text->saveFile());
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        emit s_update_tree(text->match_lines());
    }
}

void TabView::saveFileAs()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        QFileInfo fi(text->saveFileAs());
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        emit s_update_tree(text->match_lines());
    }
}

void TabView::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void TabView::reloadTabs()
{
    for(int i = 0; i  < ui->tabWidget->count(); i++) {
        if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->widget(i))) {
            text->execMatch();

            if(ui->tabWidget->widget(i)->isVisible()) {
                emit s_update_tree(text->match_lines());
            }
        }
    }
}

void TabView::goToLine(int line)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->goToLine(line);
    }
}

void TabView::findText(QString text, bool regex, bool whole_word, bool backward, bool case_sensitive)
{
    if(plainTextView *text_edit = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text_edit->findText(text, regex, whole_word, backward, case_sensitive);
    }
}


void TabView::on_tabWidget_currentChanged(int index)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        emit s_update_tree(text->match_lines());
    }
}
