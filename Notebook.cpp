#include "Notebook.h"
#include "ui_Notebook.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextStream>
#include <QClipboard>
#include <QPrinter>
#include <QPrintDialog>
#include <QProcess>
#include "FindDialog.h"
#include "FontDialog.h"
#include "NotebookSettings.h"
#include "PreferencesDialog.h"

#define UNTITLED QString("Untitled")

Notebook::Notebook(QString open_file, bool new_file, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notebook)
{
    ui->setupUi(this);

    // Setup Find/Replace Dialog
    find_dialog = new FindDialog(this);
    connect(find_dialog, &FindDialog::findNext, this, &Notebook::findNext);
    connect(find_dialog, &FindDialog::findPrev, this, &Notebook::findPrev);
    connect(find_dialog, &FindDialog::replace, this, &Notebook::replace);
    connect(find_dialog, &FindDialog::replaceAll, this, &Notebook::replaceAll);

    // Setup Clipboard
    clipboard = QGuiApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, this, &Notebook::clipboardChanged);

    // Read Settings and Apply CommandLine Args
    QFont font;
    NotebookSettings::readSettings(&m_current_file_path, &font, &m_load_recent, &m_word_wrap, &m_status_bar);
    updatePreferences(m_load_recent, m_word_wrap, m_status_bar);

    // Setup Text Edit
    ui->textEdit->setFont(font);
    if(m_current_file_path != UNTITLED && !m_current_file_path.isEmpty() && open_file.isEmpty() && !new_file && m_load_recent)
        loadFile(m_current_file_path);
    else if(!open_file.isEmpty() && !new_file)
        loadFile(open_file);
    else
        newFile();

    // Setup Status Bar
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &Notebook::cursorMoved);
}

Notebook::~Notebook()
{
    delete find_dialog;
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

void Notebook::newFile()
{
    // Prompt to save file
    if(!ui->textEdit->toPlainText().isEmpty() )
    {
        auto save = m_current_filename == UNTITLED ?
                    QMessageBox::question(this,
                              tr("Unsaved Changes"),
                              "Do you want to save these changes?") :
                    QMessageBox::question(this,
                              tr("Unsaved Changes"),
                              "Do you want to save changes to " + m_current_filename + "?");
        if(save == QMessageBox::Yes)
            saveAsFile();
    }

    // reset application
    ui->textEdit->clear();
    ui->actionCopy->setDisabled(true);
    ui->actionCut->setDisabled(true);
    ui->actionUndo->setDisabled(true);
    ui->actionRedo->setDisabled(true);
    m_current_filename = UNTITLED;
    m_current_file_path = "";
}

void Notebook::loadFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
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

    setCurrentFile(&file);
}

void Notebook::saveFile()
{
    QFile file(m_current_file_path);
    if(file.exists())   // If file exists, overwrite it
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << ui->textEdit->toPlainText();
        }
    }
    else    // If file doesn't exist, prompt user to save as a new file
    {
        if(QMessageBox::Yes == QMessageBox::question(this, tr("Save As New File?"), tr("Would you like to save this document as a new file?")))
        {
            saveAsFile();
        }
    }
}

void Notebook::saveAsFile()
{
    QFile file(QFileDialog::getSaveFileName(this,
                                       tr("Save File"),
                                       m_current_dir,
                                       tr("Text (*.txt)")));
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        setCurrentFile(&file);
    }
    else
    {
        // error case
    }
}

void Notebook::print()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() == QDialog::Accepted)
        ui->textEdit->print(&printer);
}

void Notebook::setCurrentFile(QFile *file)
{
    // Store file name and directory
    m_current_filename = file->fileName();
    QFileInfo file_info(*file);
    m_current_dir = file_info.absolutePath();
    m_current_file_path = file_info.filePath();

    setWindowTitle("Notebook - " + file_info.fileName());
}

void Notebook::on_actionOpen_triggered()
{
    openFileDialog();
}

void Notebook::on_actionExit_triggered()
{
    close();
    QCoreApplication::quit();
}

void Notebook::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void Notebook::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Notebook::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void Notebook::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void Notebook::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void Notebook::on_actionSave_triggered()
{
    saveFile();
}

void Notebook::on_actionSave_As_triggered()
{
    saveAsFile();
}

void Notebook::on_actionPrint_triggered()
{
    print();
}

void Notebook::on_actionNew_triggered()
{
    newFile();
}

void Notebook::findNext(QString text)
{
    bool success;
    if(find_dialog->isMatchCase())
        success = ui->textEdit->find(text, QTextDocument::FindCaseSensitively);
    else
        success = ui->textEdit->find(text);

    // If unsuccessful and in wrap-around mode, try again from the beginning of the document
    if(!success && find_dialog->isWrapAround())
    {
        QTextCursor cursor = QTextCursor(ui->textEdit->document());
        cursor.movePosition(QTextCursor::Start);
        ui->textEdit->setTextCursor(cursor);

        if(find_dialog->isMatchCase())
            success = ui->textEdit->find(text, QTextDocument::FindCaseSensitively);
        else
            success = ui->textEdit->find(text);
    }

    if(!success)
    {
        QMessageBox::warning(this,
                             "Notebook",
                             "Cannot find \"" + text + "\"");
    }
    else
    {
        activateWindow();   // this is done so that highlighted text is in focus. However, Enter key can no longer be used to Find Next
    }
}

void Notebook::findPrev(QString text)
{
    bool success;
    if(find_dialog->isMatchCase())
        success = ui->textEdit->find(text, QTextDocument::FindCaseSensitively | QTextDocument::FindBackward);
    else
        success = ui->textEdit->find(text, QTextDocument::FindBackward);

    // If unsuccessful and in wrap-around mode, try again from the end of the document
    if(!success && find_dialog->isWrapAround())
    {
        QTextCursor cursor = QTextCursor(ui->textEdit->document());
        cursor.movePosition(QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);

        if(find_dialog->isMatchCase())
            success = ui->textEdit->find(text, QTextDocument::FindCaseSensitively | QTextDocument::FindBackward);
        else
            success = ui->textEdit->find(text, QTextDocument::FindBackward);
    }

    if(!success)
    {
        QMessageBox::warning(this,
                             "Notebook",
                             "Cannot find \"" + text + "\"");
    }
    else
        activateWindow();   // this is done so that highlighted text is in focus. However, Enter key can no longer be used to Find Next
}

/*!
    \fn Notebook::replace(QString original, QString target)

    \brief Replaces the next instance of <original> with <target>
*/
void Notebook::replace(QString original, QString target)
{
            // If a copy of <original> isn't highlighted, find the next instance of <original>
    if(ui->textEdit->textCursor().selectedText() != original)
    {
        findNext(original);
    }
    else    // else replace it with <target> and then find next <original>
    {
        ui->textEdit->textCursor().removeSelectedText();
        ui->textEdit->textCursor().insertText(target);
        findNext(original);
    }
}

/*!
    \fn Notebook::replaceAll(QString original, QString target)

    \brief Replaces all instances of <original> with <target>
*/
void Notebook::replaceAll(QString original, QString target)
{
    // Set wrap-around to true so we comb through the whole doc
    bool is_wrap_around = find_dialog->isWrapAround();
    find_dialog->setWrapAround(true);

    while(ui->textEdit->toPlainText().contains(original))
    {
        replace(original, target);
    }

    find_dialog->setWrapAround(is_wrap_around); // return wrap-around setting to original value
}

void Notebook::clipboardChanged()
{
    ui->actionPaste->setDisabled(clipboard->text().isEmpty());
}

void Notebook::cursorMoved()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->statusbar->showMessage("Ln " + QString::number(cursor.blockNumber() + 1) + ", Col " + QString::number(cursor.positionInBlock() + 1));
}

void Notebook::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

void Notebook::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

void Notebook::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

void Notebook::on_actionFind_triggered()
{
    find_dialog->openFindDialog();
}

void Notebook::on_actionReplace_triggered()
{
    find_dialog->openReplaceDialog();
}

void Notebook::on_actionFont_triggered()
{
    FontDialog font_dialog(ui->textEdit->font(), this);
    connect(&font_dialog, &FontDialog::fontChanged, this, &Notebook::fontChanged);
    font_dialog.exec();
}

void Notebook::fontChanged(QFont font)
{
    ui->textEdit->setFont(font);
}

void Notebook::closeEvent(QCloseEvent *event)
{
    NotebookSettings::writeSettings(m_current_file_path, ui->textEdit->font(), m_load_recent, m_word_wrap, m_status_bar);
    event->accept();
}

void Notebook::on_actionNew_Window_triggered()
{
    QProcess process;
    process.startDetached("Notebook_Test.exe", {"n"});
}

void Notebook::on_actionPreferences_triggered()
{
    PreferencesDialog dialog(m_load_recent, m_word_wrap, m_status_bar, this);
    connect(&dialog, &PreferencesDialog::emitPreferences, this, &Notebook::updatePreferences);
    dialog.exec();
}

void Notebook::updatePreferences(bool load_recent, bool word_wrap, bool status_bar)
{
    m_load_recent = load_recent;
    m_word_wrap = word_wrap;
    m_status_bar = status_bar;
    if(m_word_wrap)
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    else
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);

    if(status_bar)
    {
        ui->statusbar->show();
        cursorMoved();
    }
    else
        ui->statusbar->hide();
}
