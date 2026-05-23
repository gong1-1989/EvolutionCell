#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "src/Utils/gamecanvas.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    this->setWindowTitle("进化亿重奏:细胞纪元");
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("HomePage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartGame_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("ModPage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
    }
}


void MainWindow::on_btnLoadGame_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("LoadPage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
    }
}


void MainWindow::on_btnBook_clicked()
{

    QWidget* page=ui->stackedWidget->findChild<QWidget*>("BookPage");
    if(page){
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
    }
}


void MainWindow::on_btnSetting_clicked()
{    
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("SettingPage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
    }
}


void MainWindow::on_btnExit_clicked()
{
    this->close();
}


void MainWindow::on_btnBackHome_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("HomePage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(false);
    }
}


void MainWindow::on_btnEvolveNormal_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("GamePage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);

        GameCanvas* canvas=ui->widgetCanvas;
        canvas->setFocus();
    }
}


void MainWindow::on_btnEvolveEndless_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("GamePage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
        GameCanvas* canvas=ui->widgetCanvas;
        canvas->setFocus();
    }
}


void MainWindow::on_btnTrail_clicked()
{
    QWidget* page=ui->stackedWidget->findChild<QWidget*>("GamePage");
    if(page) {
        ui->stackedWidget->setCurrentWidget(page);
        ui->btnBackHome->setVisible(true);
        GameCanvas* canvas=ui->widgetCanvas;
        canvas->setFocus();
    }
}

