#ifndef TABVIEW_H
#define TABVIEW_H

#include <QAbstractButton>
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
    void refreshFile();
    void saveFile();
    void saveFileAs();
    void reloadTabs();
    void goToLine(int line);
    void findText(QString text, bool regex, bool whole_word, bool case_sensitive, bool up, bool down, bool arround);
    void zoomIn();
    void zoomOut();
    void addBookmark();
    void cleanBookmark();
    void delBookmark();
private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_tabWidget_tabBarDoubleClicked(int index);

signals:
    void s_update_tree(const QMap<QString, QList<int>>&);
    void s_set_status(QString status, int timeout);

protected:
    /**
    * this event is called when the mouse enters the widgets area during a drag/drop operation
    */
   void dragEnterEvent(QDragEnterEvent* event);

   /**
    * this event is called when the mouse moves inside the widgets area during a drag/drop operation
    */
   void dragMoveEvent(QDragMoveEvent* event);

   /**
    * this event is called when the mouse leaves the widgets area during a drag/drop operation
    */
   void dragLeaveEvent(QDragLeaveEvent* event);

   /**
    * this event is called when the drop operation is initiated at the widget
    */
   void dropEvent(QDropEvent* event);

private:
    Ui::TabView *ui;
};

#endif // TABVIEW_H
