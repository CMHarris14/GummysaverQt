#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "util/logger.h"
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

// Add a new game
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
    // for (const Backup &backup : game_list.get_backups()) {
    // }
}

// Event when selected game changes
void MainWindow::on_list_games_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    this->working_game = ui->list_games->currentItem()->text();
    refresh_path_list();
}

// Add a directory to a games backup list
void MainWindow::on_button_addPath_clicked() {
    QString selectedPath = QFileDialog::getExistingDirectory(this, "Select Directory", QString());
    if (QFileInfo(selectedPath).isDir()) {
        this->game_list.add_path(this->working_game, selectedPath);
    } else {
        QMessageBox::warning(this, "Error", "The selected path cannot be read or does not exist");
    }
    refresh_path_list();
}

// Add a single file to a games backup list
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

void MainWindow::on_button_deletePath_clicked() {
    QList<QListWidgetItem *> selected = ui->list_paths->selectedItems();
    for (QListWidgetItem *item : selected) {
        this->game_list.delete_path(this->working_game, item->text());
    }
    refresh_path_list();
}
