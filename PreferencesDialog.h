#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(bool load_recent, bool word_wrap, bool status_bar, QWidget *parent = nullptr);
    ~PreferencesDialog();

signals:
    void emitPreferences(bool, bool, bool);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
