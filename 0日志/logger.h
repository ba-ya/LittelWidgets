#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <iostream>

namespace aux {

class Logger {
public:
    static Logger &instance() {
        static Logger instance;
        return instance;
    }
    void setLogPrefix(const QString &prefix) { log_prefix = prefix;}
    const QString &LogPrefix() { return log_prefix; }

    void EnableConsole(bool enable = true) { console_enable = enable; }
    void EnableFile(int type = 1) { file_type = type; }

    void Log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        QString txt = format_message(type, msg);
        if (console_enable) {
            std::cout << txt.toStdString() << std::endl;
        }

        if (file_type != 0) {
            QString file_name = create_log_name(log_prefix, file_type);
            QFile out_file(file_name);
            if (out_file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                QTextStream ts(&out_file);
                ts << txt << "\n";
                out_file.close();
            }
        }
    }

    QString create_log_name(const QString &prefix, int time_format) {
        static QString cachedFileName = [prefix, time_format]() {
            QString format = "yyyy_MM_dd_hh";
            if (time_format == 2) format += "_mm";
            else if (time_format == 3) format += "_mm_ss";

            auto time = QDateTime::currentDateTime();
            return QString("%1_%2.log").arg(prefix, time.toString(format));
        }();

        return cachedFileName;
    }

private:
    bool console_enable = false;
    int file_type = 1;
    QString log_prefix = "server";

    Logger() = default;

    QString format_message(QtMsgType type, const QString &msg) {
        switch (type) {
        case QtDebugMsg:
            return QString("[Debug]: %1").arg(msg);
        case QtWarningMsg:
            return QString("[Warning]: %1").arg(msg);
        case QtCriticalMsg:
            return QString("[Critical]: %1").arg(msg);
        default:
            return QString("[Fatal]: %1").arg(msg);
        }
    }
};

} // namespace aux

#endif // LOGGER_H
