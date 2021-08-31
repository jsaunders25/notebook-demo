#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QMainWindow>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class Notebook; }
QT_END_NAMESPACE

class QFile;
class FindDialog;

class Notebook : public QMainWindow
{
    Q_OBJECT

public:
    Notebook(QString open_file, bool new_file, QWidget *parent = nullptr);
    ~Notebook();

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    // File Menu Actions
    void on_actionNew_triggered();
    void on_actionNew_Window_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();

    // Edit Menu Actions
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    // Tools Menu Actions
    void on_actionPreferences_triggered();
    void updatePreferences(bool load_recent, bool word_wrap, bool status_bar);
    void on_actionFont_triggered();
    void fontChanged(QFont font);

    // Find/Replace Slots
    void findNext(QString text);
    void findPrev(QString text);
    void replace(QString original, QString target);
    void replaceAll(QString original, QString target);

    // Text Edit Slots
    void on_textEdit_redoAvailable(bool b);
    void on_textEdit_undoAvailable(bool b);
    void on_textEdit_copyAvailable(bool b);
    void clipboardChanged();

    // Status Bar Slots
    void cursorMoved();

private:
    Ui::Notebook *ui;
    FindDialog  *find_dialog;
    QClipboard  *clipboard; /**< window system clipboard */
    QString     m_current_filename; /**< only name of current file */
    QString     m_current_file_path; /**< full path of current file */
    QString     m_current_dir;  /**< directory of current file */

    void openFileDialog();
    void newFile();
    void loadFile(const QString &filename);
    void saveFile();
    void saveAsFile();
    void print();

    void setCurrentFile(QFile *file);

    // Preferences
    bool m_load_recent, m_word_wrap, m_status_bar;
};
#endif // NOTEBOOK_H
