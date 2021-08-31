#ifndef NOTEBOOKSETTINGS_H
#define NOTEBOOKSETTINGS_H

#include <QFont>
#include <QObject>

class NotebookSettings
{
public:
    NotebookSettings();
    static void writeSettings(QString last_file, QFont font, bool load_recent, bool word_wrap, bool status_bar);
    static void readSettings(QString* last_file, QFont* font, bool* load_recent, bool* word_wrap, bool* status_bar);
};

#endif // NOTEBOOKSETTINGS_H
