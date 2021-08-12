#ifndef NOTEBOOKSETTINGS_H
#define NOTEBOOKSETTINGS_H

#include <QFont>
#include <QObject>

class NotebookSettings
{
public:
    NotebookSettings();
    static void writeSettings(QString last_file, QFont font);
    static void readSettings(QString* last_file, QFont* font);

private:
    // settings to save in file
//    QFont m_font;
//    QString m_last_file;
//    QQueue<QString> m_recent_files;
//    QStringList m_file_extensions;
//    bool m_save_prompt;
//    QString m_color_scheme;
};

#endif // NOTEBOOKSETTINGS_H
