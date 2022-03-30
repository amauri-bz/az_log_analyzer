#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filetree.h"
#include "indextree.h"
#include "tabview.h"
#include "projectview.h"
#include "findview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const QString &getProj_name() const;
    void setProj_name(const QString &newProj_name);

    const QString &getProj_path() const;
    void setProj_path(const QString &newProj_path);

signals:
    void s_new_tab();
    void s_new_proj(QString proj_name, QString proj_path);
    void s_open_proj(QString proj_name, QString proj_path);
    void s_save_proj();
    void s_open_file(QString file_path);
    void s_save_file();
    void s_save_file_as();

private slots:
    void on_actionNewFile_triggered();
    void on_actionNew_Project_triggered();
    void on_actionOpenFile_triggered();
    void on_actionSaveFile_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionSave_Project_triggered();

    void on_actionSaveFileAs_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_QT_triggered();

private:
    Ui::MainWindow *ui;
    FileTree file_tree;
    IndexTree index_tree;
    TabView tab_view;
    ProjectView project_view;
    FindView find_view;
    QString proj_name;
    QString proj_path;
};
#endif // MAINWINDOW_H
