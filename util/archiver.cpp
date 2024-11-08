#include "archiver.h"
#include "exceptions.cpp"
#include "logger.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <sstream>

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

void archiver::make_backup(QString game, QString backup_name, const QStringList &paths) {
    QProcess process;

    // Format path for the backup file
    std::ostringstream backup_path;
    backup_path << backup_dir.toStdString() << game.toStdString() << "\\" << backup_name.toStdString();

    // Gather arguments for backup path and files to add to archive
    QStringList arguments;
    arguments << "a" << QString::fromStdString(backup_path.str());
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
