#include "filetree.h"
#include "ui_filetree.h"
#include <QDebug>

FileTree::FileTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FileTree)
{
    ui->setupUi(this);
}

FileTree::~FileTree()
{
    delete ui;
}

void FileTree::setProj(QString proj_path) {
    model.setFilter( QDir::AllDirs | QDir::AllEntries | QDir::NoDotAndDotDot);
    model.setRootPath(proj_path);

    ui->treeView->setModel(&model);
    ui->treeView->setRootIndex(model.index(proj_path));

    // Demonstrating look and feel features
    ui->treeView->setAnimated(false);
    ui->treeView->setIndentation(20);
    ui->treeView->setSortingEnabled(true);
    for (int i = 1; i < model.columnCount(); ++i)
        ui->treeView->hideColumn(i);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void FileTree::on_treeView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid() and model.fileInfo(index).isFile()) {
        emit s_open_file(model.fileInfo(index).absoluteFilePath());
    }
}
