#include "indextree.h"
#include "ui_indextree.h"
#include "projectmgr.h"
#include <QStandardItemModel>

/*!
    \brief Object constructor
*/
IndexTree::IndexTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::IndexTree)
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
}

/*!
    \brief Object destructor
*/
IndexTree::~IndexTree()
{
    delete ui;
}

/*!
    \brief Handle the index_tree event from tab_view to build the index tree
*/
void IndexTree::createIndex(const QMap<QString, QList<int> >& data) {
    QStandardItemModel *model = new QStandardItemModel(this);
    auto rootNode = model->invisibleRootItem();

    int count = 0;
    for(QString key : projectMgr::Instance()->proj_matchs_.keys())
    {
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

/*!
    \brief Handle double click event on the index tree and emit the go-to-line event
*/
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

