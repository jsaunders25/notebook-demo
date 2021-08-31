#include "NotebookSettings.h"
#include <QQueue>
#include <QSettings>

NotebookSettings::NotebookSettings()
{}

void NotebookSettings::writeSettings(QString last_file, QFont font, bool load_recent, bool word_wrap, bool status_bar)
{
    QSettings settings("Joshua Saunders", "Notebook");
    settings.beginGroup("Font");
    settings.setValue("Font Family", font.family());
    settings.setValue("Font Bold", font.bold());
    settings.setValue("Font Italic", font.italic());
    settings.setValue("Font Size", font.pointSize());
    settings.endGroup();

    settings.beginGroup("Recent");
    settings.setValue("Load Recent on Startup", load_recent);
    settings.setValue("Last File", last_file);
    settings.endGroup();

    settings.beginGroup("Editor");
    settings.setValue("Word Wrap", word_wrap);
    settings.setValue("Show Status Bar", status_bar);
    settings.endGroup();
}

void NotebookSettings::readSettings(QString *last_file, QFont *font, bool *load_recent, bool *word_wrap, bool *status_bar)
{
    QSettings settings("Joshua Saunders", "Notebook");
    settings.beginGroup("Font");
    font->setFamily(settings.value("Font Family", "Times New Roman").toString());
    font->setBold(settings.value("Font Bold", false).toBool());
    font->setItalic(settings.value("Font Italic", false).toBool());
    font->setPointSize(settings.value("Font Size", 12).toInt());
    settings.endGroup();

    settings.beginGroup("Recent");
    *load_recent = settings.value("Load Recent on Startup", true).toBool();
    *last_file = settings.value("Last File", "").toString();
    settings.endGroup();

    settings.beginGroup("Editor");
    *word_wrap = settings.value("Word Wrap", true).toBool();
    *status_bar = settings.value("Show Status Bar", false).toBool();
    settings.endGroup();
}
