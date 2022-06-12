#include "execcmd.h"
#include "ui_execcmd.h"

#include <QFileDialog>
#include <QProcess>

/*!
    \brief Constructing the object and initializing the program types
*/
ExecCmd::ExecCmd(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ExecCmd)
{
    ui->setupUi(this);
    ui->CmdBox->addItem("python");
    ui->CmdBox->addItem("shell");
}

/*!
    \brief Destructing the object
*/
ExecCmd::~ExecCmd()
{
    delete ui;
}

/*!
    \brief Handle the exec button click to run an external script/bin
*/
void ExecCmd::on_ExecBtn_clicked()
{
    QString  command;
    QStringList params;

    if(ui->CmdBox->currentText() == "shell") {
        command = ui->ScriptEdit->text();
        params = QStringList() << ui->ParamEdit->text().split(" ");
    }
    else if(ui->CmdBox->currentText() == "python") {
        command = "python";
        params = QStringList() << ui->ScriptEdit->text() << ui->ParamEdit->text().split(" ");
    }

    QProcess process = QProcess();
    process.start(command, params);
    process.waitForFinished();

    QString output(process.readAllStandardOutput());
    ui->OutText->setPlainText(output);
    process.close();
}

/*!
    \brief Handle the tool button to get a script full path
*/
void ExecCmd::on_ScriptTool_clicked()
{
    QString dir_path = QFileDialog::getOpenFileName(this, tr("Open File"), "/home");
    QFileInfo fi(dir_path);
    ui->ScriptEdit->setText(QString(fi.filePath()));
}

