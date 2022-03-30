#ifndef INDEXTREE_H
#define INDEXTREE_H

#include <QDockWidget>

namespace Ui {
class IndexTree;
}

class IndexTree : public QDockWidget
{
    Q_OBJECT

public:
    explicit IndexTree(QWidget *parent = nullptr);
    ~IndexTree();

public slots:
    void createIndex(const QMap<QString, QList<int>>& data);
private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
signals:
    void s_go_to_line(int line);
private:
    Ui::IndexTree *ui;
};

#endif // INDEXTREE_H
