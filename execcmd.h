#ifndef EXECCMD_H
#define EXECCMD_H

#include <QDockWidget>

namespace Ui {
class ExecCmd;
}

class ExecCmd : public QDockWidget
{
    Q_OBJECT

public:
    explicit ExecCmd(QWidget *parent = nullptr);
    ~ExecCmd();

private slots:
    void on_ExecBtn_clicked();

    void on_ScriptTool_clicked();

private:
    Ui::ExecCmd *ui;
};

#endif // EXECCMD_H
