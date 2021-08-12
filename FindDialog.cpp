#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    // Since text fields are empty, disable buttons
    ui->pushButton_find_next->setDisabled(true);
    ui->pushButton_find_prev->setDisabled(true);
    ui->pushButton_replace->setDisabled(true);
    ui->pushButton_replace_all->setDisabled(true);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::openFindDialog()
{
    setWindowTitle("Find");
    ui->label_replace->hide();
    ui->lineEdit_replace->hide();
    ui->pushButton_replace->hide();
    ui->pushButton_replace_all->hide();

    show();
}

void FindDialog::openReplaceDialog()
{
    setWindowTitle("Replace");
    ui->label_replace->show();
    ui->lineEdit_replace->show();
    ui->pushButton_replace->show();
    ui->pushButton_replace_all->show();

    show();
}

bool FindDialog::isMatchCase()
{
    return m_match_case;
}

bool FindDialog::isWrapAround()
{
    return m_wrap_around;
}

void FindDialog::setMatchCase(bool value)
{
    m_match_case = value;
}

void FindDialog::setWrapAround(bool value)
{
    m_wrap_around = value;
}

void FindDialog::on_pushButton_cancel_clicked()
{
    hide();
}

void FindDialog::on_pushButton_find_next_clicked()
{
    emit findNext(ui->lineEdit_find->text());
    setFocus();
}

void FindDialog::on_pushButton_find_prev_clicked()
{
    emit findPrev(ui->lineEdit_find->text());
    setFocus();
}

void FindDialog::on_pushButton_replace_clicked()
{
    emit replace(ui->lineEdit_find->text(), ui->lineEdit_replace->text());
    setFocus();
}

void FindDialog::on_pushButton_replace_all_clicked()
{
    emit replaceAll(ui->lineEdit_find->text(), ui->lineEdit_replace->text());
}

void FindDialog::on_checkBox_match_case_stateChanged(int arg1)
{
    m_match_case = (Qt::Checked == arg1);
}

void FindDialog::on_checkBox_wrap_around_stateChanged(int arg1)
{
    m_wrap_around = (Qt::Checked == arg1);
}

void FindDialog::on_lineEdit_find_textChanged(const QString &text)
{
    ui->pushButton_find_next->setDisabled(text.isEmpty());
    ui->pushButton_find_prev->setDisabled(text.isEmpty());
    ui->pushButton_replace->setDisabled(text.isEmpty() || ui->lineEdit_replace->text().isEmpty());
    ui->pushButton_replace_all->setDisabled(text.isEmpty() || ui->lineEdit_replace->text().isEmpty());
}

void FindDialog::on_lineEdit_replace_textChanged(const QString &text)
{
    bool disabled = (ui->lineEdit_find->text().isEmpty() || text.isEmpty());
    ui->pushButton_replace->setDisabled(disabled);
    ui->pushButton_replace_all->setDisabled(disabled);
}
