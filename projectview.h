#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QDockWidget>

namespace Ui {
class ProjectView;
}

class ProjectView : public QDockWidget
{
    Q_OBJECT

public:
    explicit ProjectView(QWidget *parent = nullptr);
    ~ProjectView();

public slots:
    void newProj(QString proj_name, QString proj_path);
    void openProj(QString proj_name, QString proj_path);
    void saveProj();

signals:
    void s_set_proj(QString proj_path);
    void s_reload();

private:
    Ui::ProjectView *ui;
    void displayFileProj(QString proj_dir, QString proj_name);
};

#endif // PROJECTVIEW_H
