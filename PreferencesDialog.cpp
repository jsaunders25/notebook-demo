#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(bool load_recent, bool word_wrap, bool status_bar, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    ui->checkBox_load_recent->setChecked(load_recent);
    ui->checkBox_word_wrap->setChecked(word_wrap);
    ui->checkBox_status_bar->setChecked(status_bar);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_buttonBox_accepted()
{
    bool load_recent = ui->checkBox_load_recent->isChecked();
    bool word_wrap = ui->checkBox_word_wrap->isChecked();
    bool status_bar = ui->checkBox_status_bar->isChecked();

    emit emitPreferences(load_recent, word_wrap, status_bar);
}
