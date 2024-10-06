#include "gamelist.h"

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
