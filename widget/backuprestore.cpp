#include "backuprestore.h"
#include "ui_backuprestore.h"

BackupRestore::BackupRestore(QWidget *parent, QList<QPair<QString, QString>> *backup_files)
    : QDialog(parent), ui(new Ui::BackupRestore) {
    ui->setupUi(this);
    QStringList labels = {"Name", "Target"};
    ui->table_paths->setColumnCount(2);
    ui->table_paths->setHorizontalHeaderLabels(labels);
    set_backup_info(backup_files);
}

BackupRestore::~BackupRestore() { delete ui; }

void BackupRestore::set_backup_info(QList<QPair<QString, QString>> *backup_files) {
    // ui->table_paths->setRowCount(0);
    for (int i = 0; i < backup_files->size(); ++i) {
        QPair<QString, QString> backup = (*backup_files)[i];

        ui->table_paths->insertRow(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(backup.second);
        QTableWidgetItem *pathItem = new QTableWidgetItem(backup.first);

        ui->table_paths->setItem(i, 0, nameItem);
        ui->table_paths->setItem(i, 1, pathItem);
    }
}
