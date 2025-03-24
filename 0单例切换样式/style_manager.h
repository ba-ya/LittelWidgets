#ifndef STYLE_MANAGER_H
#define STYLE_MANAGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QRegExp>
#include <QStringList>

class StyleManager : public QObject
{
    Q_OBJECT
public:
    enum ShowModel {
        NightTime,
        DayTime,
    };

    static StyleManager &instance();
    void switchStyle(int model);
    QString styleString(const QString &filePath, int model);

signals:
    void styleChanged(int mode);

private:
    explicit StyleManager(QObject *parent = nullptr);
};

#endif // STYLE_MANAGER_H
