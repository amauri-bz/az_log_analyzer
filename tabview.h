#ifndef TABVIEW_H
#define TABVIEW_H

#include <QFile>
#include <QMap>
#include <QWidget>

namespace Ui {
class TabView;
}

class TabView : public QWidget
{
    Q_OBJECT

public:
    explicit TabView(QWidget *parent = nullptr);
    ~TabView();

public slots:
    void newTab();
    void openFile(QString file_path);
    void saveFile();
    void saveFileAs();
    void reloadTabs();
    void goToLine(int line);
    void findText(QString text, bool regex, bool whole_word, bool backward, bool case_sensitive);
    void zoomIn();
    void zoomOut();

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);

signals:
    void s_update_tree(const QMap<QString, QList<int>>&);
    void s_set_status(QString status, int timeout);

private:
    Ui::TabView *ui;
};

#endif // TABVIEW_H
