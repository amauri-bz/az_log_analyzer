#include "tabview.h"
#include "ui_tabview.h"
#include "plainTextView.h"
#include <QElapsedTimer>
#include <QFileInfo>
#include <QSplashScreen>
#include <QtDebug>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>

/*!
    \brief Object constructor
*/
TabView::TabView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabView)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    this->setAcceptDrops(true);
}

/*!
    \brief Object destructor
*/
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

/*!
    \brief Handle a refresh file signal from main-window
*/
void TabView::refreshFile() {

    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {

        QSplashScreen splash(QPixmap(":/img/loading.png").scaled(QSize(200,200), Qt::KeepAspectRatio),
                                Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
        splash.show();
        splash.showMessage("Loading a large file...", Qt::AlignCenter, Qt::black);

        QElapsedTimer timer;
        timer.start();

        if(text && text->openFile(text->getFile_path())) {
            emit s_update_tree(text->match_lines());
            QTime timeResult(0, 0);
            timeResult=timeResult.addMSecs(timer.elapsed());
            emit s_set_status("Success operation, delta-time:" + timeResult.toString("mm:ss.zzz") , 2000);
        }
        else
            emit s_set_status("Not opened", 2000);
    }
    else
        emit s_set_status("Not opened", 2000);
}

/*!
    \brief Handle a open file signal from main-window and file-tree
*/
void TabView::openFile(QString file_path)
{
    auto text = new plainTextView();
    QFileInfo fi(file_path);

    for(int i = 0; i  < ui->tabWidget->count(); i++) {
        if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->widget(i))) {
            if(text->getFile_path() == file_path) {
                ui->tabWidget->setCurrentIndex(i);
                emit s_set_status("Success operation", 2000);
                return;
            }
        }
    }

    QSplashScreen splash(QPixmap(":/img/loading.png").scaled(QSize(200,200), Qt::KeepAspectRatio),
                            Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    splash.show();
    splash.showMessage("Loading a large file...", Qt::AlignCenter, Qt::black);

    QElapsedTimer timer;
    timer.start();

    if(text && text->openFile(file_path)) {
        ui->tabWidget->addTab(text, QString(fi.fileName()).arg(ui->tabWidget->count()+1));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QString(fi.fileName()));
        ui->tabWidget->setTabToolTip(ui->tabWidget->currentIndex(), fi.filePath());

        emit s_update_tree(text->match_lines());

        QTime timeResult(0, 0);
        timeResult=timeResult.addMSecs(timer.elapsed());
        emit s_set_status("Success operation, delta-time:" + timeResult.toString("mm:ss.zzz") , 2000);
    }
    else
        emit s_set_status("Not opened", 2000);
}

/*!
    \brief Handle a save file signal from main-window
*/
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

/*!
    \brief Handle a save-file-as signal from main-window
*/
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

/*!
    \brief Handle close tabbutton click event
*/
void TabView::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);

    if(!ui->tabWidget->count()) {
        QMap<QString, QList<int>> empty;
        emit s_update_tree(empty);
    }
}

/*!
    \brief Handle a reload all tabs signal from project view
*/
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

/*!
    \brief Handle a go-to-line signal from index view
*/
void TabView::goToLine(int line)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->goToLine(line);
        emit s_set_status("Success operation", 2000);
    }
    else
        emit s_set_status("Line not found", 2000);
}

/*!
    \brief Handle a find signal from find view
*/
void TabView::findText(QString text, bool regex, bool whole_word, bool case_sensitive, bool up, bool down, bool arround)
{
    if(plainTextView *text_edit = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        if(text_edit->findText(text, regex, whole_word, case_sensitive, up, down, arround)) {
            emit s_set_status("Success operation", 2000);
        }
        else {
            emit s_set_status("Not found", 2000);
        }
    }
}

/*!
    \brief Handle tab changed event
*/
void TabView::on_tabWidget_currentChanged(int index)
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        emit s_update_tree(text->match_lines());
    }
}

/*!
    \brief Handle zoom singnal from main-window
*/
void TabView::zoomIn() {
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->zoomIn();
    }
}

/*!
    \brief Handle zoom singnal from main-window
*/
void TabView::zoomOut() {
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->zoomOut();
    }
}

/*!
    \brief Handle bookmark singnal from main-window
*/
void TabView::addBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->addBookmark();
        emit s_update_tree(text->match_lines());
    }
}

/*!
    \brief Handle bookmark singnal from main-window
*/
void TabView::delBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->delBookmark();
        emit s_update_tree(text->match_lines());
    }
}

/*!
    \brief Handle bookmark singnal from main-window
*/
void TabView::cleanBookmark()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(ui->tabWidget->currentWidget())) {
        text->cleanBookmark();
        emit s_update_tree(text->match_lines());
    }
}

/*!
    \brief Handle double click tab event
*/
void TabView::on_tabWidget_tabBarDoubleClicked(int index)
{
    newTab();
}

/*!
    \brief Handle drag enter event
*/
void TabView::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

/*!
    \brief Handle drag move event
*/
void TabView::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

/*!
    \brief Handle drag leave event
*/
void TabView::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

/*!
    \brief Handle drop event
*/
void TabView::dropEvent(QDropEvent* event)
{
   const QMimeData* mimeData = event->mimeData();

   if (mimeData->hasUrls())
   {
     QList<QUrl> urlList = mimeData->urls();

     for (int i = 0; i < urlList.size() && i < 32; ++i)
     {
        openFile(urlList.at(i).toLocalFile());
     }
     event->acceptProposedAction();
   }
}
