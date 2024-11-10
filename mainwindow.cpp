#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "util/logger.h"
#include "widget/backuprestore.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    build_button_list();
    QStringList labels = {"Name", "Date"};
    ui->list_backups->setHorizontalHeaderLabels(labels);
    refresh_game_list();
    if (ui->list_games->count() > 0) {
        working_game = ui->list_games->item(0)->text();
    } else {
        disable_buttons();
    }
}

MainWindow::~MainWindow() { delete ui; }

// Add game button
void MainWindow::on_button_addGame_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Game", "Enter game name", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        try {
            this->game_list.add_game(name);
        } catch (const CustomException &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    } else if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name cannot be empty");
    }
    this->refresh_game_list();
}

void MainWindow::refresh_game_list() {
    ui->list_games->clear();
    ui->list_games->addItems(this->game_list.get_game_titles());
}

void MainWindow::refresh_path_list() {
    ui->list_paths->clear();
    for (const QString &path : game_list.get_paths(this->working_game)) {
        ui->list_paths->addItem(path);
    }
    enable_buttons();
}

void MainWindow::refresh_backup_list() {
    ui->list_backups->setRowCount(0);
    QList<Backup> backup_files = game_list.get_game_backups(this->working_game);
    for (int i = 0; i < backup_files.size(); ++i) {
        const Backup backup = backup_files[i];

        ui->list_backups->insertRow(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(backup.name);
        QTableWidgetItem *dateItem = new QTableWidgetItem(backup.time.toString("yyyy-MM-dd HH:mm"));

        ui->list_backups->setItem(i, 0, nameItem);
        ui->list_backups->setItem(i, 1, dateItem);
    }
}

// Selected game changes
void MainWindow::on_list_games_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    this->working_game = ui->list_games->currentItem()->text();
    refresh_path_list();
    refresh_backup_list();
}

// Add directory button
void MainWindow::on_button_addPath_clicked() {
    QString selectedPath = QFileDialog::getExistingDirectory(this, "Select Directory", QString());
    if (QFileInfo(selectedPath).isDir()) {
        this->game_list.add_path(this->working_game, selectedPath);
    } else {
        QMessageBox::warning(this, "Error", "The selected path cannot be read or does not exist");
    }
    refresh_path_list();
}

// Add file button
void MainWindow::on_button_addFile_clicked() {
    QString selectedFile =
        QFileDialog::getOpenFileName(this, "Select File or Folder", QString(), "All Files (*)");

    if (QFileInfo(selectedFile).isFile()) {
        this->game_list.add_path(this->working_game, selectedFile);
    } else {
        QMessageBox::warning(this, "Error", "The file path cannot be read or does not exist.");
    }
    refresh_path_list();
}

// Delete path button
void MainWindow::on_button_deletePath_clicked() {
    QList<QListWidgetItem *> selected = ui->list_paths->selectedItems();
    for (QListWidgetItem *item : selected) {
        this->game_list.delete_path(this->working_game, item->text());
    }
    refresh_path_list();
}

// Add backup button
void MainWindow::on_button_addBackup_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Backup", "Enter backup name", QLineEdit::Normal, "", &ok);

    // Make sure backup name is unique to prevent file conflicts
    QList<Backup> backup_files = game_list.get_game_backups(this->working_game);
    for (Backup &backup : backup_files) {
        if (backup.name == name) {
            QMessageBox::warning(this, "Warning", "Backup name must be unique");
            return;
        }
    }
    // Only create the backup if the name isn't an empty string
    if (ok && !name.isEmpty()) {
        try {
            this->game_list.make_game_backup(working_game, name);
        } catch (const CustomException &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    } else if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name cannot be empty");
    }
    this->refresh_backup_list();
}

// Delete backup button
void MainWindow::on_button_deleteBackup_clicked() {
    int row = ui->list_backups->currentRow();
    if (row == -1)
        return;

    QTableWidgetItem *nameItem = ui->list_backups->item(row, 0);
    if (nameItem) {
        game_list.delete_game_backup(this->working_game, nameItem->text());
    }
    refresh_backup_list();
}

void MainWindow::on_button_renameBackup_clicked() {}

void MainWindow::on_button_restoreBackup_clicked() {
    int row = ui->list_backups->currentRow();
    if (row == -1)
        return;

    QTableWidgetItem *nameItem = ui->list_backups->item(row, 0);
    if (nameItem) {
        QList<QPair<QString, QString>> tmp =
            game_list.get_backup_metadata(this->working_game, nameItem->text());

        BackupRestore restore_dialog(this, &tmp);

        if (restore_dialog.exec() == QDialog::Accepted) {
            game_list.restore_backup(this->working_game, nameItem->text(), tmp);
        }
    }
}
