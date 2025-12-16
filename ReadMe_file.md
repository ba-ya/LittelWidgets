- [获取该路径目录/文件, 不带全路径](#获取该路径目录文件-不带全路径)
- [如果文件(目录)不存在创建文件(目录)](#如果文件目录不存在创建文件目录)
- [删除目录下所有文件](#删除目录下所有文件)
- [删除某文件](#删除某文件)
- [重名目录](#重名目录)


# 获取该路径目录/文件, 不带全路径

```c++
// 获取该路径下所有目录
auto list_dir = QDir(dir_chosen).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
// 获取该路径下所有文件
auto list_file = QDir(dir_chosen).entryList(QDir::Files);

QStringList filters = { "*.*"};
auto list_file = QDir(dir_shot).entryList(filters, QDir::Files, QDir::Name);
```

```c++
// 显示在表格中
 for (int i = 0; i < cnt_row; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(dirList.at(i));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(i, 0, item);
    }
```

# 如果文件(目录)不存在创建文件(目录)

```c++
// 文件
        QFile file(setting_name);
        if (!file.exists()) {
            if (file.open(QIODevice::WriteOnly)) {
                qDebug() << "File created successfully:" << setting_name;
            } else {
                qWarning() << "Failed to create file:" << setting_name;
            }
        }
// 目录
        QDir directory;
        if (!directory.exists(dir_name)) {
            if (directory.mkpath(dir_name)) {
                qDebug() << "Directory created successfully:" << dir_name;
            } else {
                qDebug() << "Failed to create directory:" << dir_name;
            }
        }
```

# 删除目录及其所有文件

```c++
QString dir;
QDir(dir).removeRecursively();
```

# 删除目录下所有文件

```c++
void remove_all_files_from(QString &dir_name) {
        auto d = QDir(dir_name);
        auto f = d.entryList(QDir::Files);
        for (auto &a : f) {
            d.remove(a);
        }
    }
```

# 删除某文件

```c++
file.remove()
```

# 拷贝

```
QFile(name_src).copy(name_dst)
```

| 功能                             | QDir 示例                                     | QFileInfo 示例                                     | 说明                                                 |
| -------------------------------- | --------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------- |
| 获取当前目录绝对路径             | QDir dir("/path/to/dir"); dir.absolutePath(); | QFileInfo("/path/to/dir/file.txt").absolutePath(); | QDir 是目录路径；QFileInfo 获取文件所在目录路径      |
| 拼接子路径                       | dir.filePath("child.txt")                     | QFileInfo(basePath + "/child.txt")                 | QDir 自动处理分隔符；QFileInfo 需手动拼接            |
| 拼接子路径（绝对路径）           | dir.absoluteFilePath("child.txt")             | —                                                  | 推荐 QDir 用于路径拼接                               |
| 获取目录名（最后一级）           | dir.dirName()                                 | QFileInfo("/a/b/c/").fileName();                   | 目录路径最后一级的名称                               |
| 获取文件名（含扩展名）           | —                                             | QFileInfo("a/b.txt").fileName();                   | 返回 b.txt                                           |
| 获取文件名（不含扩展名）         | —                                             | QFileInfo("a/b.txt").baseName();                   | 返回 b                                               |
| 获取完整 baseName（支持.tar.gz） | —                                             | QFileInfo("a.tar.gz").completeBaseName();          | 返回 a.tar（不包括最后一个扩展名）                   |
| 获取扩展名                       | —                                             | QFileInfo("a/b.txt").suffix();                     | 返回 txt                                             |
| 判断路径是否存在                 | dir.exists()                                  | QFileInfo("a.txt").exists();                       | QDir 适合判断目录是否存在；QFileInfo 可判断文件/目录 |
| 判断目录是否包含某个文件         | dir.exists("target.txt")                      | —                                                  | QDir 会拼出完整路径检查                              |
| 获取文件大小（字节）             | —                                             | QFileInfo("a.txt").size();                         | 返回字节大小                                         |
| 判断是否是目录                   | dir.exists() && dir.isReadable()              | QFileInfo("a").isDir();                            | QFileInfo 更清晰                                     |
| 判断是否是文件                   | —                                             | QFileInfo("a.txt").isFile();                       |                                                      |
| 获取上一级目录                   | dir.cdUp(); dir.path();                       | QFileInfo("a/b.txt").absoluteDir().path();         | QDir 可以 cdUp()，QFileInfo 可取 absoluteDir()       |
| 获取完整路径（含文件名）         | —                                             | QFileInfo("a/b.txt").absoluteFilePath();           | 完整路径 + 文件名                                    |
| 获取相对路径                     | QDir::relativeFilePath(fullPath)              | —                                                  | 需提供 base QDir 与目标路径                          |





# 重名目录

```c++
    // 处理重名目录, 后面加"(%1)",
    // 如果有缺失的目录,先填充缺失目录
    void adjust_base_name(const QString &dir_load, QString &base_name) {
        auto &&get_version_status_from_dir = [&](const QString &dir_load,
                                          const QString &base_name) {
            QDir directory(dir_load);

            // 获取所有子目录
            auto dirList = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
            std::map<int, bool> version_status;

            for (const auto &a : dirList) {
                QString dir_name = a.fileName();

                if (dir_name.startsWith(base_name)) {
                    QRegularExpression regex("\\((\\d+)\\)");  // 匹配 "(数字)" 的格式
                    QRegularExpressionMatch match = regex.match(dir_name);

                    if (dir_name == base_name) {
                        version_status[0] = true;
                    }
                    if (match.hasMatch()) {
                        int version = match.captured(1).toInt();
                        version_status[version] = true;
                    }
                }
            }
            // 确定最大版本号
            int max_version = 0;
            if (!version_status.empty()) {
                max_version = version_status.rbegin()->first;  // 反向迭代获取最大版本号
            }

            // 填充缺失的版本
            for (int i = 0; i <= max_version; ++i) {
                if (version_status.find(i) == version_status.end()) {
                    version_status[i] = false;  // 如果某版本号没有出现，标记为缺失
                }
            }

            return version_status;
        };

        auto version_status = get_version_status_from_dir(dir_load, base_name);
        auto is_missing = false;
        if (!version_status.empty()) {
            // missing "save(0)" = missing "save"
            if (version_status[0] == false) {
                return;
            }

            auto max_version = version_status.rbegin()->first;
            for (int i = 0; i <= max_version; ++i) {
                if (version_status[i] == false) {
                    is_missing = true;
                    base_name += QString("(%1)").arg(i);
                    break;
                }
            }
            if (!is_missing) {
                base_name += QString("(%1)").arg(max_version + 1);
            }
        }
    }
```

