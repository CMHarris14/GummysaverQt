#include "gamelist.h"
#include "../util/logger.h"

GameList::GameList() {}

void GameList::add_game(QString title) {
    if (!get_game_titles().contains(title)) {
        GameHandler new_game;
        new_game.title = title;
        this->game_list.push_back(new_game);
    } else {
        throw CustomException(CustomException::DuplicateTitle);
    }
}

QVector<QString> GameList::get_game_titles() {
    QVector<QString> titles;
    for (GameHandler &game : this->game_list) {
        titles.push_back(game.title);
    }
    return titles;
}

std::optional<GameHandler> GameList::game_by_title(QString title) {
    for (int i = 0; i < game_list.length(); i++) {
        if (game_list[i].title == title) {
            return game_list[i];
        }
    }
    return std::nullopt;
}

std::optional<GameHandler> GameList::game_at(int index) {
    if (index < game_list.length()) {
        return game_list[index];
    }
    return std::nullopt;
}

void GameList::save_database() {
    try {
        SQLite::Database db("GameData.db3",
                            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    } catch (SQLite::Exception e) {
        Logger::log(Logger::ERROR, e.what());
    }
}
