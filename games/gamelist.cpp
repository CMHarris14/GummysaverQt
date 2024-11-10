#include "gamelist.h"
#include "../util/logger.h"
#include <QDir>
#include <QFileInfo>

GameList::GameList() {}

// Add a new game. Name must be unique in the database.
void GameList::add_game(const QString &title) {
    QStringList current_games = database.get_games();
    if (!current_games.contains(title)) {
        database.add_game(title.toStdString());
    } else {
        throw CustomException::DuplicateTitle;
    }
}

// Delete a game from the database by name
void GameList::delete_game(const QString &title) { database.delete_game(title.toStdString()); }

// Get a list of all the game titles in the database
QStringList GameList::get_game_titles() { return database.get_games(); }

// Add a backup path for a file/directory to the given game
void GameList::add_path(const QString &game, const QString &path) {
    database.add_path(game.toStdString(), path.toStdString());
}

// Delete a specific path from the game
void GameList::delete_path(const QString &game, const QString &path) {
    database.delete_path(game.toStdString(), path.toStdString());
}

// Get all backup file/directory paths for a game
QStringList GameList::get_paths(const QString &game) { return database.get_paths(game.toStdString()); }

// Get the path to the games image if it exists
std::optional<QString> GameList::get_image_path(const QString &game) {
    QString image_path = database.get_image_path(game.toStdString());
    if (QFileInfo(image_path).isFile()) {
        return image_path;
    } else {
        return {};
    }
}

// Update the path to a games image assuming the file exists
void GameList::update_game_image(const QString &game, const QString &path) {
    database.update_game_image(game.toStdString(), path.toStdString());
}

void GameList::make_game_backup(const QString &title, const QString &backup_name) {
    archives.make_backup(title, backup_name, get_paths(title));
}

QList<Backup> GameList::get_game_backups(const QString &title) {
    QString dir = archives.backup_dir + "\\" + title + "\\";
    QList<Backup> backup_list;
    QDir working_dir(dir);

    working_dir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = working_dir.entryInfoList();

    for (const QFileInfo &fileInfo : fileInfoList) {
        Backup new_backup(fileInfo.fileName().chopped(3), fileInfo.birthTime());
        backup_list.append(new_backup);
    }

    return backup_list;
}

void GameList::delete_game_backup(const QString &title, const QString &backup_name) {
    QString file_path = archives.backup_dir + title + "\\" + backup_name + ".7z";
    QFile file(file_path);
    file.remove();
}

QList<QPair<QString, QString>> GameList::get_backup_metadata(const QString &game,
                                                             const QString &backup_name) {
    return archives.get_path_info(game, backup_name);
}

void GameList::restore_backup(const QString &game, const QString &backup_name,
                              const QList<QPair<QString, QString>> &file_pairs) {
    archives.extract_backup(game, backup_name);
    QDir temp_dir("temp");
    if (!temp_dir.exists()) {
        Logger::log(Logger::ERROR,
                    "'temp' directory does not exist, but it should. This is likely a 7zip error");
        return;
    }

    // Process and move each pair as either file or directory
    for (const QPair<QString, QString> &pair : file_pairs) {
        QString source_path = ".\\temp\\" + pair.second;
        QString target_path = pair.first;

        QFileInfo source_file(source_path);

        if (!source_file.exists()) {
            qDebug() << "Source does not exist: " << source_path;
            continue;
        }

        // If it's a file
        if (source_file.isFile()) {
            qDebug() << "File path is " << source_path;
            QFile file(source_path);
            QFile target(target_path);
            if (target.exists()) {
                target.remove();
            }
            if (file.rename(target_path)) {
                QString tmp = "Successfully moved file: " + source_path + "to" + target_path;
                Logger::log(Logger::INFO, tmp.toStdString());
            } else {
                QString tmp = "Failed to moved file: " + source_path;
                Logger::log(Logger::WARN, tmp.toStdString());
            }
        }

        // If it's a folder
        else if (source_file.isDir()) {
            move_directory(source_path, target_path);
        }
    }

    // Cleanup
    QDir temp("temp");
    temp.removeRecursively();
}

void GameList::move_directory(const QString &source_path, const QString &target_path) {
    QDir source_dir(source_path);
    QDir target_dir(target_path);

    // Check if the source directory exists
    if (!source_dir.exists()) {
        qDebug() << "Source directory does not exist!";
        return;
    }

    // If the target directory exists, remove it recursively
    if (target_dir.exists()) {
        if (!target_dir.removeRecursively()) {
            qDebug() << "Failed to remove target directory: " << target_path;
            return;
        } else {
            qDebug() << "Target directory removed: " << target_path;
        }
    }

    // Create target directory again (after removal)
    if (!target_dir.mkpath(".")) {
        qDebug() << "Failed to create target directory: " << target_path;
        return;
    }

    // Get the list of files and subdirectories in the source directory
    QStringList entries = source_dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &entry, entries) {
        QString source_file_path = source_path + "/" + entry;
        QString target_file_path = target_path + "/" + entry;

        QFileInfo file_info(source_file_path);

        // If it's a directory, recursively move it
        if (file_info.isDir()) {
            move_directory(source_file_path, target_file_path);
        } else if (file_info.isFile()) {
            // If it's a file, move it using rename
            if (QFile::rename(source_file_path, target_file_path)) {
                qDebug() << "Successfully moved file: " << source_file_path << " to " << target_file_path;
            } else {
                qDebug() << "Failed to move file: " << source_file_path << " to " << target_file_path;
            }
        }
    }
}
