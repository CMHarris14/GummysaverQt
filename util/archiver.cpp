#include "archiver.h"
#include "exceptions.cpp"
#include "logger.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <stdexcept>
// #include <sstream>

archiver::archiver() {
    // Set correct command to use depending on OS
    if (QOperatingSystemVersion::currentType() == QOperatingSystemVersion::Windows) {
        zipper = "7zr.exe";
        Logger::log(Logger::INFO, "Running on Windows. Using 7zr.exe.");
    } else {
        zipper = "7z";
        Logger::log(Logger::INFO, "Running on non-Windows system. Using 7z.");
    }
}

void archiver::make_backup(const QString &game, const QString &backup_name, const QStringList &paths) {
    QProcess process;

    // Format path for the backup file
    QString backup_path = backup_dir + game + "\\" + backup_name;
    // Gather arguments for backup path and files to add to archive
    QStringList arguments;
    arguments << "a" << backup_path;
    for (const QString &path : paths) {
        arguments << path;
    }
    arguments << "META";

    // Set process data
    process.setProgram(zipper);
    process.setArguments(arguments);

    qDebug() << "Compressing with arguments:";
    for (QString &arg : arguments) {
        qDebug() << arg;
    }

    create_metadata(paths);

    // Run 7z and pray it works
    process.start();
    process.waitForFinished();

    QFile file("META");
    file.remove();

    if (process.exitStatus() != QProcess::NormalExit) {
        QString message = "7zip compression failed with error " + QString::number(process.exitCode());
        Logger::log(Logger::ERROR, message.toStdString());
        throw(CustomException::CompressionFailure);
    }
}

// Gets the META file from a backup archive and returns a list of pairs relating
// the backup files to their original paths
QList<QPair<QString, QString>> archiver::get_path_info(const QString &game, const QString &backup_name) {
    QProcess process;

    QString backup_path = backup_dir + game + "\\" + backup_name + ".7z";
    QStringList arguments;
    arguments << "e" << "-y" << backup_path << "META";

    // Set process data
    process.setProgram(zipper);
    process.setArguments(arguments);

    qDebug() << "Fetching META with arguments:";
    for (QString &arg : arguments) {
        qDebug() << arg;
    }

    // Start META extraction
    process.start();
    process.waitForFinished();

    if (process.exitStatus() != QProcess::NormalExit) {
        QString message = "7zip compression failed with error " + QString::number(process.exitCode());
        Logger::log(Logger::ERROR, message.toStdString());
        throw(CustomException::CompressionFailure);
    }

    // Check if the file exists
    QFile file("META");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw("Cannot read META file");
    }

    QList<QPair<QString, QString>> path_list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue; // Skip empty lines. Should never happen but just in case

        // Split at :
        int last_colon = line.lastIndexOf(':'); // Stupid windows making this harder than needed
        QString full_path = line.left(last_colon);
        QString file_name = line.right(line.size() - last_colon - 1);
        path_list.append(qMakePair(full_path, file_name));
    }

    file.remove();
    return path_list;
}

void archiver::extract_backup(const QString &game, const QString &backup_name) {
    QProcess process;

    QString backup_path = backup_dir + game + "\\" + backup_name + ".7z";
    QStringList arguments;
    arguments << "x" << "-y" << backup_path << "-o./temp";

    process.setProgram(zipper);
    process.setArguments(arguments);

    qDebug() << "Extracting backup with arguments:";
    for (QString &arg : arguments) {
        qDebug() << arg;
    }

    process.start();
    process.waitForFinished();

    if (process.exitStatus() != QProcess::NormalExit) {
        QString message = "7zip compression failed with error " + QString::number(process.exitCode());
        Logger::log(Logger::ERROR, message.toStdString());
        throw(CustomException::CompressionFailure);
    }
}

// Create a file recording the original full path for each file in the archive for recovery later
void archiver::create_metadata(const QStringList &paths) {
    QFile file("META");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw(CustomException::NoWriteAccess);
    }

    QTextStream out(&file);
    for (const QString &path : paths) {
        QFileInfo fileInfo(path);
        if (fileInfo.exists()) {
            out << path << ":" << fileInfo.fileName() << "\n";
        }
    }
}
