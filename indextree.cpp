#include "indextree.h"
#include "ui_indextree.h"
#include "projectmgr.h"
#include <QStandardItemModel>

IndexTree::IndexTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::IndexTree)
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
}

IndexTree::~IndexTree()
{
    delete ui;
}

void IndexTree::createIndex(const QMap<QString, QList<int> >& data) {
    QStandardItemModel *model = new QStandardItemModel(this);
    auto rootNode = model->invisibleRootItem();

    int count = 0;
    for(QString key : projectMgr::Instance()->proj_matchs_.keys())
    {
        qInfo() << "createIndex Key: " << key;

        if(data.value(key).count()<=0)
            continue;

        auto item = new QStandardItem(key);
        item->setBackground(QColor(projectMgr::Instance()->proj_matchs_.value(key)));
        item->setEditable(false);

        for(int val : data.value(key))
        {
            auto subitem = new QStandardItem(QString::number(val));
            subitem->setEditable(false);
            item->appendRow(subitem);
        }

        rootNode->appendRow(item);
        count++;
    }
    ui->treeView->setModel(model);
}

void IndexTree::on_treeView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid()) {
        bool ok;
        int num = index.data().toInt(&ok);
        if (ok) {
          emit s_go_to_line(num);
        }
    }
}

