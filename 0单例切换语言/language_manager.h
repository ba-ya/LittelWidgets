#ifndef LANGUAGE_MANAGER_H
#define LANGUAGE_MANAGER_H

#include <QObject>
#include <QTranslator>

class LanguageManager : public QObject
{
    Q_OBJECT
public:
    enum Lang{
        ENGLISH,
        CHINESE,
    };

    static LanguageManager &instance(); // Singleton pattern
    void switchLanguage(int lang);

signals:
    void languageChanged(int lang);

private:
    explicit LanguageManager(QObject *parent = nullptr);
    QTranslator *translator;
};

#endif // LANGUAGE_MANAGER_H
