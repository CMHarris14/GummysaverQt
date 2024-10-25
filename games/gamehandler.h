#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QDateTime>
#include <QFile>
#include <QPixMap>

struct Backup {
    Backup(QString name, QDateTime time) {
        this->name = name;
        this->time = time;
    }
    QString name;
    QDateTime time;
};

class GameHandler {
  private:
    QPixmap *boxart;
    QString boxart_path;
    QVector<QString> paths;
    QVector<Backup> backup_list;

  public:
    GameHandler();
    QString title;
    // Update the games box are with the image at the given path
    void update_boxart(QString imgpath);

    // Add and remove files/folders for this game to backup
    void add_path(QString path);
    void remove_path(int index);
    QVector<QString> get_paths();
    // Add and remove game backups
    void add_backup(QString name);
    void remove_backup(int index);
    QVector<Backup> get_backups();
    Backup &backup_at(int index);
};

#endif // GAMEHANDLER_H
