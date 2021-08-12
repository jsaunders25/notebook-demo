#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    // options to open dialog
    void openFindDialog();
    void openReplaceDialog();

    // getters
    bool isMatchCase();
    bool isWrapAround();

    // setters
    void setMatchCase(bool value);
    void setWrapAround(bool value);

signals:
    void findNext(QString);
    void findPrev(QString);
    void replace(QString, QString);     // <original text>,<target text>
    void replaceAll(QString, QString);  // <original text>,<target text>

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_find_next_clicked();
    void on_pushButton_find_prev_clicked();
    void on_pushButton_replace_clicked();
    void on_pushButton_replace_all_clicked();

    void on_checkBox_match_case_stateChanged(int arg1);
    void on_checkBox_wrap_around_stateChanged(int arg1);

    void on_lineEdit_find_textChanged(const QString &text);
    void on_lineEdit_replace_textChanged(const QString &text);

private:
    Ui::FindDialog *ui;
    bool m_match_case = false;
    bool m_wrap_around = false;
};

#endif // FINDDIALOG_H
