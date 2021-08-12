#include "NotebookSettings.h"
#include <QQueue>
#include <QSettings>

NotebookSettings::NotebookSettings()
{}

void NotebookSettings::writeSettings(QString last_file, QFont font)
{
    QSettings settings("Joshua Saunders", "Notebook");
    settings.beginGroup("Font");
    settings.setValue("Font Family", font.family());
    settings.setValue("Font Bold", font.bold());
    settings.setValue("Font Italic", font.italic());
    settings.setValue("Font Size", font.pointSize());
    settings.endGroup();

    settings.beginGroup("Recent");
    settings.setValue("Last File", last_file);
    settings.endGroup();
}

void NotebookSettings::readSettings(QString *last_file, QFont *font)
{
    QSettings settings("Joshua Saunders", "Notebook");
    settings.beginGroup("Font");
    font->setFamily(settings.value("Font Family", "Times New Roman").toString());
    font->setBold(settings.value("Font Bold", false).toBool());
    font->setItalic(settings.value("Font Italic", false).toBool());
    font->setPointSize(settings.value("Font Size", 12).toInt());
    settings.endGroup();

    settings.beginGroup("Recent");
    *last_file = settings.value("Last File", "").toString();
    settings.endGroup();
}
