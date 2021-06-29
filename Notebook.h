#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QMainWindow>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class Notebook; }
QT_END_NAMESPACE

class Notebook : public QMainWindow
{
    Q_OBJECT

public:
    Notebook(QWidget *parent = nullptr);
    ~Notebook();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::Notebook *ui;
    QString     m_current_file; // full path of current file
    QString     m_current_dir;  // directory of current file
    QSet<int>   m_pressed_keys; // keys currently pressed

    void openFileDialog();
    void loadFile(const QString &filename);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
#endif // NOTEBOOK_H
