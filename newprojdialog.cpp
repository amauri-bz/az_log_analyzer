#include "newprojdialog.h"
#include "ui_newprojdialog.h"
#include <QFileDialog>

/*!
    \brief Object constructor
*/
NewProjDialog::NewProjDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjDialog)
{
    ui->setupUi(this);
}

/*!
    \brief Object destructor
*/
NewProjDialog::~NewProjDialog()
{
    delete ui;
}

/*!
    \brief Project path getter
*/
const QString &NewProjDialog::getProj_path() const
{
    return proj_path;
}

/*!
    \brief Project path setter
*/
void NewProjDialog::setProj_path(const QString &newProj_path)
{
    proj_path = newProj_path;
}

/*!
    \brief Project name getter
*/
const QString &NewProjDialog::getProj_name() const
{
    return proj_name;
}

/*!
    \brief Project name setter
*/
void NewProjDialog::setProj_name(const QString &newProj_name)
{
    proj_name = newProj_name;
}

/*!
    \brief Handle tool button click event
*/
void NewProjDialog::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    setProj_path(dir);
    ui->path_txt->setText(dir);
}

/*!
    \brief Handle cancel button click event
*/
void NewProjDialog::on_cancel_btn_clicked()
{
    QDialog::reject();
}

/*!
    \brief Handle create button click event
*/
void NewProjDialog::on_create_btn_clicked()
{
    setProj_name(ui->name_txt->text());
    setProj_path(ui->path_txt->text());

    if(getProj_name().isEmpty()) {
        ui->statusbar->setText("Invalid project name");
        return;
    }
    if(getProj_path().isEmpty()) {
        ui->statusbar->setText("Invalid project path");
        return;
    }
    ui->statusbar->setText("");
    QDialog::accept();
}

