#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/exceptions.cpp"
#include "gamehandler.h"

#include <SQLiteCpp/SQLiteCpp.h>

class GameList {
  private:
    QVector<GameHandler> game_list;

  public:
    GameList();
    // Overloads to treat the class as a vector
    GameHandler &operator[](int index) { return game_list[index]; }
    auto begin() { return game_list.begin(); }
    auto end() { return game_list.end(); }

    // Add a new game to manage. Titles must be unique
    void add_game(QString title);
    // Returns the titles of every managed game
    QVector<QString> get_game_titles();

    std::optional<std::reference_wrapper<GameHandler>>
    game_by_title(QString title);

    std::optional<int> index_by_title(QString title);

    void save_database();
};

#endif // GAMELIST_H
