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
    fillTemplateBox();
}

/*!
    \brief Object destructor
*/
NewProjDialog::~NewProjDialog()
{
    delete ui;
}

/*!
    \brief Fill tamplate combo box
*/
void NewProjDialog::fillTemplateBox()
{
    ui->templateBox->addItem("");

    QDir directory(QDir::currentPath()+"/templates");

    if (!directory.exists()) {
        ui->statusbar->setText("Template folder not exist");
        return;
    }

    QStringList files = directory.entryList(QStringList() << "*.json",QDir::Files);
    foreach(QString filename, files) {
        ui->templateBox->addItem(filename);
    }
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

const QString &NewProjDialog::getProj_template() const
{
    return proj_template;
}

void NewProjDialog::setProj_template(const QString &newProj_template)
{
    proj_template = newProj_template;
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
    setProj_template(ui->templateBox->currentText());

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

