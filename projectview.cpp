#include "projectview.h"
#include "ui_projectview.h"
#include <QDebug>
#include <QDir>
#include <QTextEdit>
#include "projectmgr.h"
#include "plaintextview.h"

ProjectView::ProjectView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ProjectView)
{
    ui->setupUi(this);
    plainTextView *txt = new plainTextView();
    txt->enableToolBtn(true);
    this->setWidget(txt);
}

ProjectView::~ProjectView()
{
    delete ui;
}

void ProjectView::displayFileProj(QString proj_dir, QString proj_name) {
    QString file(proj_dir + "/" + proj_name + ".json");
    if(plainTextView *text = qobject_cast<plainTextView*>(this->widget())) {
        text->setFile(file);
    }
}

void ProjectView::newProj(QString proj_name, QString proj_path)
{
    qInfo() << "New Proj OK " << proj_name << ", " << proj_path;

    QString proj_dir(proj_path + "/" + proj_name);

    if(!QDir(proj_dir).exists()) {
        QDir().mkdir(proj_dir);
        qInfo() << "creating proj dir";

        projectMgr::Instance()->CreateProjFile(proj_dir, proj_name);
        displayFileProj(proj_dir, proj_name);
        emit s_set_proj(proj_dir);
    }
}

void ProjectView::openProj(QString proj_name, QString proj_path)
{
    qInfo() << "Open Proj OK " << proj_name << ", " << proj_path;

    if(QDir(proj_path).exists()) {
        projectMgr::Instance()->ReadProjFile(proj_path, proj_name);
        displayFileProj(proj_path, proj_name);
        emit s_set_proj(proj_path);
    }
}

void ProjectView::saveProj()
{
    if(plainTextView *text = qobject_cast<plainTextView*>(this->widget())) {
        text->saveFile();
        emit s_reload();
    }
}