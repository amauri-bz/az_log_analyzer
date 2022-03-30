#ifndef FILETREE_H
#define FILETREE_H

#include <QDockWidget>
#include <QFileSystemModel>

namespace Ui {
class FileTree;
}

class FileTree : public QDockWidget
{
    Q_OBJECT

public:
    explicit FileTree(QWidget *parent = nullptr);
    ~FileTree();

public slots:
    void setProj(QString proj_path);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

signals:
    void s_open_file(QString file_path);

private:
    Ui::FileTree *ui;
    QFileSystemModel model;
};

#endif // FILETREE_H
