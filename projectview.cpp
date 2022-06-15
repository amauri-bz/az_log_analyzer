#include "projectview.h"
#include "ui_projectview.h"
#include <QDebug>
#include <QDir>
#include <QTextEdit>
#include "projectmgr.h"
#include "plaintextview.h"

/*!
    \brief Object constructor
*/
ProjectView::ProjectView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ProjectView)
{
    ui->setupUi(this);
    plainTextView *txt = new plainTextView();
    txt->enableToolBtn(true);
    this->setWidget(txt);
}

/*!
    \brief Object destructor
*/
ProjectView::~ProjectView()
{
    delete ui;
}

void ProjectView::displayFileProj(QString proj_dir, QString proj_name) {
    QString file(proj_dir + "/" + proj_name + ".json");
    if(plainTextView *text = qobject_cast<plainTextView*>(this->widget())) {
        text->openFile(file);
    }
}

/*!
    \brief Handle new project signal from main-window
*/
void ProjectView::newProj(QString proj_name, QString proj_path, QString proj_template)
{
    QString proj_dir(proj_path + "/" + proj_name);

    if(!QDir(proj_dir).exists()) {
        QDir().mkdir(proj_dir);

        projectMgr::Instance()->CreateProjFile(proj_dir, proj_name, proj_template);
        displayFileProj(proj_dir, proj_name);
        emit s_set_proj(proj_dir);
        emit s_set_status("Success operation", 2000);
    }
    else {
        emit s_set_status("Project Already exist", 2000);
    }
}

/*!
    \brief Handle open project signal from main-window
*/
void ProjectView::openProj(QString proj_name, QString proj_path)
{
    if(QDir(proj_path).exists()) {
        projectMgr::Instance()->ReadProjFile(proj_path, proj_name);
        displayFileProj(proj_path, proj_name);
        emit s_set_proj(proj_path);
        emit s_reload();
        emit s_set_status("Success operation", 2000);
    }
    else {
        emit s_set_status("Invalid path", 2000);
    }
}

/*!
    \brief Handle save project signal from main-window
*/
void ProjectView::saveProj()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(this->widget())) {
        text->saveFile();
        emit s_reload();
        emit s_set_status("Success operation", 2000);
    }
    else {
        emit s_set_status("Project not saved", 2000);
    }
}
