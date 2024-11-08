#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/archiver.h"
#include "../util/exceptions.cpp"
#include "../util/gamedb.h"
#include <QDateTime>
// #include "gamehandler.h"

#include <SQLiteCpp/SQLiteCpp.h>

struct Backup {
    Backup(QString name, QDateTime time) {
        this->name = name;
        this->time = time;
    }
    QString name;
    QDateTime time;
};

class GameList {
  private:
    gamedb database;
    archiver archives;

  public:
    GameList();

    void add_game(QString &title);
    void delete_game(QString &title);
    QStringList get_game_titles();

    void add_path(QString &game, QString &path);
    void delete_path(const QString &game, const QString &path);
    QStringList get_paths(QString &game);

    std::optional<QString> get_image_path(QString &game);
    void update_game_image(QString &game, QString &path);

    void make_game_backup(QString &title, QString &backup_name);
    void delete_game_backup(QString &title, const QString &backup_name);
    QList<Backup> get_game_backups(QString &title);
};

#endif // GAMELIST_H
