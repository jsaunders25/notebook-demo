#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QDialog>

namespace Ui {
class FontDialog;
}

class FontDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontDialog(QFont font, QWidget *parent = nullptr);
    ~FontDialog();

signals:
    void fontChanged(QFont font);

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_comboBox_style_currentIndexChanged(const QString &style);
    void on_comboBox_size_currentIndexChanged(const QString &size);

    void on_buttonBox_accepted();

private:
    Ui::FontDialog *ui;
    void populateComboBoxes();
};

#endif // FONTDIALOG_H
