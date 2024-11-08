#include "gamelist.h"
#include "../util/logger.h"
#include <QDir>
#include <QFileInfo>

GameList::GameList() {}

// Add a new game. Name must be unique in the database.
void GameList::add_game(QString &title) {
    QStringList current_games = database.get_games();
    if (!current_games.contains(title)) {
        database.add_game(title.toStdString());
    } else {
        throw CustomException::DuplicateTitle;
    }
}

// Delete a game from the database by name
void GameList::delete_game(QString &title) { database.delete_game(title.toStdString()); }

// Get a list of all the game titles in the database
QStringList GameList::get_game_titles() { return database.get_games(); }

// Add a backup path for a file/directory to the given game
void GameList::add_path(QString &game, QString &path) {
    database.add_path(game.toStdString(), path.toStdString());
}

// Delete a specific path from the game
void GameList::delete_path(const QString &game, const QString &path) {
    database.delete_path(game.toStdString(), path.toStdString());
}

// Get all backup file/directory paths for a game
QStringList GameList::get_paths(QString &game) { return database.get_paths(game.toStdString()); }

// Get the path to the games image if it exists
std::optional<QString> GameList::get_image_path(QString &game) {
    QString image_path = database.get_image_path(game.toStdString());
    if (QFileInfo(image_path).isFile()) {
        return image_path;
    } else {
        return {};
    }
}

// Update the path to a games image assuming the file exists
void GameList::update_game_image(QString &game, QString &path) {
    database.update_game_image(game.toStdString(), path.toStdString());
}

void GameList::make_game_backup(QString &title, QString &backup_name) {
    archives.make_backup(title, backup_name, get_paths(title));
}

QList<Backup> GameList::get_game_backups(QString &title) {
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

void GameList::delete_game_backup(QString &title, const QString &backup_name) {
    QString file_path = archives.backup_dir + title + "\\" + backup_name + ".7z";
    QFile file(file_path);
    file.remove();
}
