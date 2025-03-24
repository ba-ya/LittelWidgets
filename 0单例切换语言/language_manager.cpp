#include "language_manager.h"
#include <QApplication>

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent), translator(new QTranslator(this)) { }

LanguageManager &LanguageManager::instance()
{
    static LanguageManager instance;
    return instance;
}

void LanguageManager::switchLanguage(int lang)
{
    switch(lang) {
    case ENGLISH:
        translator->load(":/translations/en_US.qm");
        break;
    case CHINESE:
        translator->load(":/translations/zh_CN.qm");
        break;

    default:
        translator->load(":/translations/en_US.qm");
        break;
    }
    qApp->installTranslator(translator);

    emit languageChanged(lang);
}
