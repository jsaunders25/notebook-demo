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
    Notebook(QWidget *parent = nullptr);
    ~Notebook();

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    // Menu Actions
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionNew_triggered();

    // Editor Slots
    void findNext(QString text);
    void findPrev(QString text);
    void replace(QString original, QString target);
    void replaceAll(QString original, QString target);

    void clipboardChanged();

    void on_textEdit_redoAvailable(bool b);
    void on_textEdit_undoAvailable(bool b);
    void on_textEdit_copyAvailable(bool b);

    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    void on_actionFont_triggered();
    void fontChanged(QFont font);

private:
    Ui::Notebook *ui;
    FindDialog  *find_dialog;
    QClipboard  *clipboard;     // window system clipboard
    QString     m_current_filename; // only name of current file
    QString     m_current_file_path; // full path of current file
    QString     m_current_dir;  // directory of current file

    void openFileDialog();
    void newFile();
    void loadFile(const QString &filename);
    void saveFile();
    void saveAsFile();
    void print();

    void setCurrentFile(QFile *file);
};
#endif // NOTEBOOK_H
