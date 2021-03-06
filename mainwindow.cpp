#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newprojdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

/*!
    \brief Object constructor
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->addDockWidget(Qt::LeftDockWidgetArea, &file_tree);
    this->setCentralWidget(&tab_view);
    this->addDockWidget(Qt::LeftDockWidgetArea, &find_view);

    this->tabifyDockWidget(&file_tree, &index_tree);
    this->tabifyDockWidget(&file_tree, &exec_cmd);
    this->tabifyDockWidget(&file_tree, &project_view);

    QObject::connect(this, &MainWindow::s_new_tab, &tab_view, &TabView::newTab);
    QObject::connect(this, &MainWindow::s_new_proj, &project_view, &ProjectView::newProj);
    QObject::connect(this, &MainWindow::s_open_proj, &project_view, &ProjectView::openProj);
    QObject::connect(&project_view, &ProjectView::s_set_proj, &file_tree, &FileTree::setProj);
    QObject::connect(&project_view, &ProjectView::s_reload, &tab_view, &TabView::reloadTabs);
    QObject::connect(this, &MainWindow::s_save_proj, &project_view, &ProjectView::saveProj);
    QObject::connect(&file_tree, &FileTree::s_open_file, &tab_view, &TabView::openFile);
    QObject::connect(this, &MainWindow::s_refresh_file, &tab_view, &TabView::refreshFile);
    QObject::connect(this, &MainWindow::s_open_file, &tab_view, &TabView::openFile);
    QObject::connect(this, &MainWindow::s_save_file, &tab_view, &TabView::saveFile);
    QObject::connect(this, &MainWindow::s_save_file_as, &tab_view, &TabView::saveFileAs);
    QObject::connect(this, &MainWindow::s_zoom_in, &tab_view, &TabView::zoomIn);
    QObject::connect(this, &MainWindow::s_zoom_out, &tab_view, &TabView::zoomOut);
    QObject::connect(&tab_view, &TabView::s_update_tree, &index_tree, &IndexTree::createIndex);
    QObject::connect(&index_tree, &IndexTree::s_go_to_line, &tab_view, &TabView::goToLine);
    QObject::connect(&find_view, &FindView::s_find_text, &tab_view, &TabView::findText);

    //StatusBar connection
    QObject::connect(&project_view, &ProjectView::s_set_status, this, &MainWindow::on_actionSeteStatus);
    QObject::connect(&tab_view, &TabView::s_set_status, this, &MainWindow::on_actionSeteStatus);
    this->setAcceptDrops(true);

    //Bookmark
    QObject::connect(this, &MainWindow::s_add_bookmark, &tab_view, &TabView::addBookmark);
    QObject::connect(this, &MainWindow::s_del_bookmark, &tab_view, &TabView::delBookmark);
    QObject::connect(this, &MainWindow::s_clean_bookmark, &tab_view, &TabView::cleanBookmark);
}

/*!
    \brief Object destructor
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
    \brief Handle signals from other modules to display the status
*/
void MainWindow::on_actionSeteStatus(QString status, int timeout) {
    statusBar()->showMessage(status, timeout);
}

/*!
    \brief Handle new file button click
*/
void MainWindow::on_actionNewFile_triggered()
{
    emit s_new_tab();
}

/*!
    \brief Handle new project button click
*/
void MainWindow::on_actionNew_Project_triggered()
{
    auto dialog = NewProjDialog();
    dialog.exec();

    setProj_name(dialog.getProj_name());
    setProj_path(dialog.getProj_path());
    emit s_new_proj(getProj_name(), getProj_path(), dialog.getProj_template());
}

/*!
    \brief Get proj path getter
*/
const QString &MainWindow::getProj_path() const
{
    return proj_path;
}

/*!
    \brief Get proj path setter
*/
void MainWindow::setProj_path(const QString &newProj_path)
{
    proj_path = newProj_path;
}

/*!
    \brief Get proj name getter
*/
const QString &MainWindow::getProj_name() const
{
    return proj_name;
}

/*!
    \brief Get proj name setter
*/
void MainWindow::setProj_name(const QString &newProj_name)
{
    proj_name = newProj_name;
}

/*!
    \brief Handle open file button click
*/
void MainWindow::on_actionOpenFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", proj_path,  tr("All Files (*);;*.json;;*.txt;;*.log"));
    emit s_open_file(fileName);
}

/*!
    \brief Handle save file button click
*/
void MainWindow::on_actionSaveFile_triggered()
{
    emit s_save_file();
}

/*!
    \brief Handle open project button click
*/
void MainWindow::on_actionOpen_Project_triggered()
{
    QString dir_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
               "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QFileInfo fi(dir_path);
    setProj_name(QString(fi.fileName()));
    setProj_path(dir_path);
    emit s_open_proj(getProj_name(), getProj_path());
}

/*!
    \brief Handle save project button click
*/
void MainWindow::on_actionSave_Project_triggered()
{
    if(proj_path.isEmpty()) {
        statusBar()->showMessage(tr("Project not created/opened"), 2000);
        return;
    }

    emit s_save_proj();
}

/*!
    \brief Handle save-file-as button click
*/
void MainWindow::on_actionSaveFileAs_triggered()
{
    if(proj_path.isEmpty()) {
        statusBar()->showMessage(tr("Project not created/opened"), 2000);
        return;
    }

    emit s_save_file_as();
}

/*!
    \brief Handle about button click
*/
void MainWindow::on_actionAbout_triggered()
{
    QString translatedTextAboutCaption;
    translatedTextAboutCaption = QMessageBox::tr(
        "<h3>About Az_Log_Analyser</h3>"
        "<p>This program uses Qt version %1. See About QT for more information</p>"
        ).arg(QLatin1String(QT_VERSION_STR));
    QString translatedTextAboutText;
    translatedTextAboutText = QMessageBox::tr(
        "<p>Az_Log_Analyser is cross-platform application for log analysis</p>");
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About Az_Log_Analyser"));
    msgBox->setText(translatedTextAboutCaption);
    msgBox->setInformativeText(translatedTextAboutText);
    msgBox->show();
}

/*!
    \brief Handle about QT button click
*/
void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox msg;
    msg.aboutQt(this);
}

/*!
    \brief Handle exit button click
*/
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/*!
    \brief Handle zoon in button click
*/
void MainWindow::on_actionZoom_In_triggered()
{
    emit s_zoom_in();
}

/*!
    \brief Handle zoon out button click
*/
void MainWindow::on_actionZoom_Out_triggered()
{
    emit s_zoom_out();
}

/*!
    \brief Handle add bookmark button click
*/
void MainWindow::on_actionAdd_Bookmark_triggered()
{
    emit s_add_bookmark();
}

/*!
    \brief Handle delete bookmark button click
*/
void MainWindow::on_actionDelete_Bookmark_triggered()
{
    emit s_del_bookmark();
}

/*!
    \brief Handle clear all bookmarks button click
*/
void MainWindow::on_actionClean_All_Bookmarks_triggered()
{
    emit s_clean_bookmark();
}


/*!
    \brief Handle refresh button click
*/
void MainWindow::on_actionRefresh_File_triggered()
{
    emit s_refresh_file();
}

