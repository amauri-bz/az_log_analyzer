#include "findview.h"
#include "ui_findview.h"

FindView::FindView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FindView)
{
    ui->setupUi(this);
}

FindView::~FindView()
{
    delete ui;
}

void FindView::on_okButton_clicked()
{
    bool regex = ui->RegexBox->isChecked();
    bool whole_word = ui->WholeWordBox->isChecked();
    bool case_sensitive = ui->CaseSensitiveBox->isChecked();
    QString text = ui->InputEdit->text();

    if(!text.isEmpty()) {
        emit s_find_text(text, regex, whole_word, case_sensitive);
    }

}

