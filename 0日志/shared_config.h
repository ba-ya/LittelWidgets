#ifndef SHARED_CONFIG_H
#define SHARED_CONFIG_H

#include "logger.h"

#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QDateTime>

namespace aux {

class ConfigOutput
{
public:
    static ConfigOutput &instance() {
        static ConfigOutput instance;
        return instance;
    }

    ConfigOutput(const ConfigOutput&) = delete;
    ConfigOutput& operator=(const ConfigOutput&) = delete;

    void config_define() {
        QString file_config = "./config_output.ini";
        QFile file(file_config);
        if (file.exists()) {
            load(file_config);
        }
        save(file_config);
        add_comments(file_config);

        remove_duplicate_log();
    }

    void remove_duplicate_log() {
        QString file_name = Logger::instance().create_log_name(Logger::instance().LogPrefix(), dump_log_type);
        if (QFile::exists(file_name)) {
            QFile::remove(file_name);
        }
    }

    void save(QString &file) {
        QSettings settings(file, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.clear();

        settings.beginGroup("hardware");
        settings.setValue("device_type", device_type);
        settings.setValue("scan_type", scan_type);
        settings.endGroup();

        settings.beginGroup("network");
        settings.setValue("port_srv_rev_param", port_srv_rev_param);
        settings.setValue("port_srv_send_data", port_srv_send_data);
        settings.setValue("port_clt_rev_data", port_clt_rev_data);
        settings.endGroup();

        settings.beginGroup("debug");
        settings.setValue("debug_send_info", is_debug_send_info);
        settings.endGroup();

        settings.beginGroup("dump");
        settings.setValue("dump_log_type", dump_log_type);
        settings.endGroup();
    }

    void add_comments(QString &filePath) {
        if (!QFile::exists(filePath)) {
            qWarning() << "File not found:" << filePath;
            return;
        }

        auto &&read_file = [](const QString &filePath) {
            static const QMap<QString, QString> comments = {
                {"device_type",  "#0-1:USB/PCIE"},
                {"scan_type",    "#0-1:PA/UT"},
                {"dump_log_type","#0-3:off/hh/mm/ss"},
            };
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Failed to open file for reading:" << filePath;
                return QString();
            }

            QStringList content_list;
            QTextStream in(&file);

            while (!in.atEnd()) {
                QString line = in.readLine();
                content_list << line;

                for (const auto &key : comments.keys()) {
                    if (line.startsWith(key + "=")) {
                        content_list << "       " + comments[key];
                    }
                }
            }
            return content_list.join("\n");
        };

        QString content = read_file(filePath);
        if (!content.isEmpty()) {
            write_file(filePath, content);
        }
    }

    void write_file(const QString &filePath, const QString &content) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Failed to open file for writing:" << filePath;
            return;
        }
        QTextStream out(&file);
        out << content;
    };

    void load(QString &file) {
        auto &&limit_range = [](auto v, auto min, auto max) {
            if (v < min) {
                v = min;
            }
            if (v > max) {
                v = max;
            }
            return v;
        };

        QSettings settings(file, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        settings.beginGroup("hardware");
        device_type = settings.value("device_type", device_type).toInt();
        scan_type = settings.value("scan_type", scan_type).toInt();
        settings.endGroup();

        settings.beginGroup("network");
        port_srv_rev_param = settings.value("port_srv_rev_param", port_srv_rev_param).toUInt();
        port_srv_send_data = settings.value("port_srv_send_data", port_srv_send_data).toUInt();
        port_clt_rev_data = settings.value("port_clt_rev_data", port_clt_rev_data).toUInt();
        settings.endGroup();

        settings.beginGroup("debug");
        is_debug_send_info = settings.value("debug_send_info", is_debug_send_info).toBool();
        settings.endGroup();

        settings.beginGroup("dump");
        dump_log_type = settings.value("dump_log_type", dump_log_type).toInt();
        settings.endGroup();

        device_type = limit_range(device_type, 0, 1);
        scan_type = limit_range(scan_type, 0, 1);
        dump_log_type = limit_range(dump_log_type, 0, 3);
    }

    void debug_info() {
        QStringList device = {"USB", "PCIE"};
        QStringList scan = {"PA", "UT"};
        QStringList log_type = {"off", "hh", "mm", "ss"};

        qDebug() << "===========================> config output Begin";
        qDebug().noquote() << "device:" << device[device_type];
        qDebug().noquote() << "scan:" << scan[scan_type];
        qDebug().noquote() << QString("param: clt'port(xxxx) ====> srv'port(%1)").arg(port_srv_rev_param);
        qDebug().noquote() << QString(" data: clt'port(%1) <==== srv'port(%2)").arg(port_clt_rev_data).arg(port_srv_send_data);
        qDebug() << "debug_send_info:" << is_debug_send_info;
        qDebug().noquote() << "dump_log_type:" << log_type[dump_log_type];
        qDebug() << "===========================> config output End";
    }

    int device_type = 1;// 0-1:USB/PCIE
    int scan_type = 0;// 0-1:PA/UT

    bool is_debug_kv_param = true;
    bool is_debug_hal_param = false;
    bool is_debug_send_info = false;

    bool is_dump_origin_bin = false;
    int dump_log_type = 1;// 0-3: off/hh/mm/ss

    quint16 port_srv_rev_param = 1001;
    quint16 port_srv_send_data = 2001;
    quint16 port_clt_rev_data = 2000;

private:
    ConfigOutput() {}
};

} // namespace aux

#endif // SHARED_CONFIG_H
