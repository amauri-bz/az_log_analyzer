#ifndef NEWPROJDIALOG_H
#define NEWPROJDIALOG_H

#include <QDialog>

namespace Ui {
class NewProjDialog;
}

class NewProjDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjDialog(QWidget *parent = nullptr);
    ~NewProjDialog();

    const QString &getProj_name() const;
    const QString &getProj_path() const;
    const QString &getProj_template() const;

private slots:
    void on_toolButton_clicked();
    void on_cancel_btn_clicked();
    void on_create_btn_clicked();

private:
    void setProj_name(const QString &newProj_name);
    void setProj_path(const QString &newProj_path);
    void setProj_template(const QString &newProj_template);
    void fillTemplateBox();

    Ui::NewProjDialog *ui;
    QString proj_name;
    QString proj_path;
    QString proj_template;
};

#endif // NEWPROJDIALOG_H
