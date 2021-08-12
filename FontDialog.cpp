#include "FontDialog.h"
#include "ui_FontDialog.h"

FontDialog::FontDialog(QFont font, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontDialog)
{
    ui->setupUi(this);
    populateComboBoxes();
    ui->fontComboBox->setCurrentFont(font);
    ui->comboBox_size->setCurrentIndex(ui->comboBox_size->findText(QString::number(font.pointSize())));
    QString style;
    if(!font.bold() && !font.italic())
        style = "Regular";
    else if(font.bold() && !font.italic())
        style = "Bold";
    else if(font.italic() && !font.bold())
        style = "Italic";
    else
        style = "Bold Italic";
    ui->comboBox_style->setCurrentIndex(ui->comboBox_style->findText(style));
}

FontDialog::~FontDialog()
{
    delete ui;
}

void FontDialog::populateComboBoxes()
{
    QStringList sizes;
    sizes << "8" << "9" << "10" << "11" << "12" << "14" << "16" << "18" << "20" <<
             "22" << "24" << "26" << "28" << "36" << "48";
    ui->comboBox_size->addItems(sizes);

    QStringList styles;
    styles << "Regular" << "Bold" << "Italic" << "Bold Italic";
    ui->comboBox_style->addItems(styles);
}

void FontDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont font = ui->lineEdit_sample->font();
    font.setFamily(f.family());
    ui->lineEdit_sample->setFont(font);
}

void FontDialog::on_comboBox_style_currentIndexChanged(const QString &style)
{
    QFont font = ui->lineEdit_sample->font();
    font.setBold(style.contains("Bold"));
    font.setItalic(style.contains("Italic"));
    ui->lineEdit_sample->setFont(font);
}

void FontDialog::on_comboBox_size_currentIndexChanged(const QString &size)
{
    QFont font = ui->lineEdit_sample->font();
    int font_size = size.toInt();
    font.setPointSize(font_size);
    ui->lineEdit_sample->setFont(font);
}

void FontDialog::on_buttonBox_accepted()
{
    emit fontChanged(ui->lineEdit_sample->font());
}
