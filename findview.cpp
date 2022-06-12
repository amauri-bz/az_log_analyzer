#include "findview.h"
#include "ui_findview.h"

/*!
    \brief Object constructor
*/
FindView::FindView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FindView)
{
    ui->setupUi(this);
}

/*!
    \brief Object destructor
*/
FindView::~FindView()
{
    delete ui;
}

/*!
    \brief Handle the find button click and emit a find-signal to the plain-text editor
*/
void FindView::on_okButton_clicked()
{
    bool regex = ui->RegexBox->isChecked();
    bool whole_word = ui->WholeWordBox->isChecked();
    bool case_sensitive = ui->CaseSensitiveBox->isChecked();
    bool up = ui->radioUp->isChecked();
    bool down = ui->radioDown->isChecked();
    bool arround = ui->radioArround->isChecked();
    QString text = ui->InputEdit->text();

    if(!text.isEmpty()) {
        emit s_find_text(text, regex, whole_word, case_sensitive, up, down, arround);
    }

}

