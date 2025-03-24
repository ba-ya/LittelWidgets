#include <QApplication>
#include "style_manager.h"

// 静态单例实现
StyleManager &StyleManager::instance()
{
    static StyleManager instance;
    return instance;
}

StyleManager::StyleManager(QObject *parent)
    : QObject(parent)
{
}

// 切换样式
void StyleManager::switchStyle(int model)
{
    QString qss = ":/style.qss";
    auto style_sheet = StyleManager::instance().styleString(qss, model);
    qApp->setStyleSheet(style_sheet);

    emit styleChanged(model);
}

// 样式文件处理
QString StyleManager::styleString(const QString &filePath, int model)
{
    QStringList styleStr;
    QFile file(filePath);

    // 区分白天和夜间的标记
    QString styleModel = (static_cast<ShowModel>(model) == ShowModel::DayTime) ? "/*am*/" : "/*pm*/";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString lineStr(line.data());
        lineStr.remove(QRegExp("[\t\r\n]")); // 移除制表符、换行符等无效字符

        if (lineStr.contains("/*")) {
            // 如果包含指定模式的标记，则保留该行
            if (lineStr.contains(styleModel)) {
                lineStr.remove(styleModel);
                styleStr << lineStr;
            }
            continue;
        }

        if (lineStr.isEmpty()) {
           continue;
        }

        styleStr << lineStr; // 保留非空行
    }

    file.close();
    return styleStr.join("\n");
}
