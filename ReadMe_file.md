- [获取该路径目录/文件, 不带全路径](#获取该路径目录文件-不带全路径)
- [如果文件(目录)不存在创建文件(目录)](#如果文件目录不存在创建文件目录)
- [删除目录下所有文件](#删除目录下所有文件)


### 获取该路径目录/文件, 不带全路径

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

### 如果文件(目录)不存在创建文件(目录)

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

### 删除目录下所有文件

```c++
void remove_all_files_from(QString &dir_name) {
        auto d = QDir(dir_name);
        auto f = d.entryList(QDir::Files);
        for (auto &a : f) {
            d.remove(a);
        }
    }
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