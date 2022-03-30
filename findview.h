#ifndef FINDVIEW_H
#define FINDVIEW_H

#include <QDockWidget>

namespace Ui {
class FindView;
}

class FindView : public QDockWidget
{
    Q_OBJECT

public:
    explicit FindView(QWidget *parent = nullptr);
    ~FindView();

private slots:
    void on_okButton_clicked();

signals:
    void s_find_text(QString text, bool regex, bool whole_word, bool backward, bool case_sensitive);

private:
    Ui::FindView *ui;
};

#endif // FINDVIEW_H
