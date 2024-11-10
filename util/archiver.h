#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <QOperatingSystemVersion>
#include <QPair>
#include <QProcess>

class archiver {
  public:
    archiver();
    const QString backup_dir = ".\\backups\\";
    void make_backup(const QString &game, const QString &backup_name, const QStringList &paths);
    QList<QPair<QString, QString>> get_path_info(const QString &game, const QString &backup_name);
    void extract_backup(const QString &game, const QString &backup_name);

  private:
    QString zipper;
    void create_metadata(const QStringList &paths);
};

#endif // ARCHIVER_H
