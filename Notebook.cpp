#include "Notebook.h"
#include "ui_Notebook.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextStream>

Notebook::Notebook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notebook)
{
    ui->setupUi(this);  // creates instance of the widget
}

Notebook::~Notebook()
{
    delete ui;
}

void Notebook::openFileDialog()
{
    // User prompted  to select a file
    QString selected_file = QFileDialog::getOpenFileName(this,
                                                         "Open File",
                                                         m_current_dir,
                                                         tr("Text (*.txt)"));

    // If user successfully selects a file, load it
    if(!selected_file.isNull() || !selected_file.isEmpty())
        loadFile(selected_file);
}

void Notebook::loadFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadWrite | QFile::Text))
    {
        // Display a warning if the file is not successfully opened
        QMessageBox::warning(this,
                             tr("Error Opening File"),
                             tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(filename), file.errorString()));
        return;
    }

    // Setup text stream to read contents of file
    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    ui->textEdit->setPlainText(in.readAll());

    setWindowTitle("Notebook - " + filename);

    // Store file name and directory
    m_current_file = filename;
    QFileInfo file_info(file);
    m_current_dir = file_info.absolutePath();
}

void Notebook::keyPressEvent(QKeyEvent *event)
{
    m_pressed_keys.insert(event->key());

    // List of Keyboard Shortcuts
    if(m_pressed_keys.contains(Qt::Key_Control) && m_pressed_keys.contains(Qt::Key_O))
    {
        openFileDialog();
        m_pressed_keys.clear();
    }
}

void Notebook::keyReleaseEvent(QKeyEvent *event)
{
    m_pressed_keys.remove(event->key());
}

void Notebook::on_actionOpen_triggered()
{
    openFileDialog();
}
