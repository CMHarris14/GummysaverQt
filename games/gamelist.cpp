#include "gamelist.h"
#include "../util/logger.h"

GameList::GameList() {}

void GameList::add_game(QString &title) {
    QVector<QString> current_games = database.get_games();
    if (!current_games.contains(title)) {
        database.add_game(title.toStdString());
    } else {
        throw CustomException::DuplicateTitle;
    }
}

void GameList::delete_game(QString &title) { database.delete_game(title.toStdString()); }

void GameList::add_path(QString &game, QString &path) {
    database.add_path(game.toStdString(), path.toStdString());
}

void GameList::delete_path(const QString &game, const QString &path) {
    database.delete_path(game.toStdString(), path.toStdString());
}

QVector<QString> GameList::get_paths(QString &game) { return database.get_paths(game.toStdString()); }

QString GameList::get_image_path(QString &game) { return database.get_image_path(game.toStdString()); }

void GameList::update_game_image(QString &game, QString &path) {
    database.update_game_image(game.toStdString(), path.toStdString());
}

QVector<QString> GameList::get_game_titles() { return database.get_games(); }
