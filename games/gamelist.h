#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/archiver.h"
#include "../util/exceptions.cpp"
#include "../util/gamedb.h"
#include <QDateTime>

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
    void move_directory(const QString &source_path, const QString &target_path);

  public:
    GameList();

    void add_game(const QString &title);
    void delete_game(const QString &title);
    QStringList get_game_titles();

    void add_path(const QString &game, const QString &path);
    void delete_path(const QString &game, const QString &path);
    QStringList get_paths(const QString &game);

    std::optional<QString> get_image_path(const QString &game);
    void update_game_image(const QString &game, const QString &path);

    void make_game_backup(const QString &title, const QString &backup_name);
    void delete_game_backup(const QString &title, const QString &backup_name);
    QList<Backup> get_game_backups(const QString &title);
    QList<QPair<QString, QString>> get_backup_metadata(const QString &game, const QString &backup_name);
    void restore_backup(const QString &game, const QString &backup_name,
                        const QList<QPair<QString, QString>> &file_pairs);
};

#endif // GAMELIST_H
