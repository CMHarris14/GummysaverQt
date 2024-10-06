#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "util/logger.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

// Add a new game
void MainWindow::on_button_addGame_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Game", "Enter game name",
                                         QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        try {
            this->game_list.add_game(name);
            this->refresh_game_list();
        } catch (const CustomException &e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    } else if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name cannot be empty");
    }
}

void MainWindow::refresh_game_list() {
    ui->list_games->clear();
    ui->list_games->addItems(this->game_list.get_game_titles());
}

void MainWindow::on_list_games_currentItemChanged(QListWidgetItem *current,
                                                  QListWidgetItem *previous) {}

void MainWindow::on_button_addPath_clicked() {
    // this->game_list.save_database();
    Logger::log(Logger::INFO, "Nice test");
}
