[TOC]

## `注意`

```bash
#include <Windows.h>
#include <DbgHelp.h>
```

这个顺序不能反,DbgHelp用了很多windows里面的宏

## `xx.pro`

```bash
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
LIBS += -lDbgHelp
```

# +=gui

## `main.h`

```cpp
#include <Windows.h>
#include <DbgHelp.h>
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){
    qDebug() << __FUNCTION__ << "gogogo";
    //创建 Dump 文件
    QString str =QDateTime::currentDateTime().toString("yyyyMMdd")
                  + QTime::currentTime().toString("HHmmsszzz") + ".dmp";
    std::wstring wlpstr = str.toStdWString();
    HANDLE hDumpFile = CreateFile(wlpstr.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        // Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal,
                          &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16)),
        errAdr(QString::number((uint)record->ExceptionAddress, 16)), errMod;
    QMessageBox::critical(NULL, "critical", "Crashed happend", QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
```

# -=gui

## `main.h`

```cpp
#include <Windows.h>
#include <DbgHelp.h>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException) {
    qDebug() << __FUNCTION__ << "Crash handler triggered";

    // Create Dump file name with current date and time
    QString str = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz") + ".dmp";
    std::wstring wlpstr = str.toStdWString();

    // Create Dump file
    HANDLE hDumpFile = CreateFile(wlpstr.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        // Dump information
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;

        // Write Dump file content
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal,
                          &dumpInfo, NULL, NULL);

        CloseHandle(hDumpFile); // Close the file handle after writing
    }

    // Handle crash without using QMessageBox
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode = QString::number(record->ExceptionCode, 16);
    QString errAdr = QString::number(reinterpret_cast<quintptr>(record->ExceptionAddress), 16);

    qDebug() << "Error Code:" << errCode << "Address:" << errAdr;
    qDebug() << "Crash occurred, exiting application.";

    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
```

