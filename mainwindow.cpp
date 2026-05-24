#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "src/Utils/gamecanvas.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_currSelectSlot(-1)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    this->setWindowTitle("进化亿重奏:细胞纪元");
    ui->stackedWidget->setCurrentWidget(ui->HomePage);
    ui->btnBackHome->setVisible(false);
    ui->tableSaveList->setColumnCount(3);
    ui->tableSaveList->setHorizontalHeaderLabels({"存档时间","吞噬总数","细胞体型"});
    ui->tableSaveList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableSaveList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_rightMenu=new QMenu(this);
    QAction* actDel=m_rightMenu->addAction("删除存档");
    connect(actDel,&QAction::triggered,this,&MainWindow::deleteSelectedSave);
    connect(ui->tableSaveList,&QTableWidget::customContextMenuRequested,this,&MainWindow::showSaveRightMenu);
    ui->btnLoadGame->setEnabled(hasAnyValidSave());
    refreshSaveTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartGame_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ModPage);
    ui->btnBackHome->setVisible(true);
}


void MainWindow::on_btnLoadGame_clicked()
{
    refreshSaveTable();
    ui->stackedWidget->setCurrentWidget(ui->LoadPage);
    ui->btnBackHome->setVisible(true);
}


void MainWindow::on_btnBook_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->BookPage);
    ui->btnBackHome->setVisible(true);
}


void MainWindow::on_btnSetting_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->SettingPage);
     ui->btnBackHome->setVisible(true);
}


void MainWindow::on_btnExit_clicked()
{
    this->close();
}


void MainWindow::on_btnBackHome_clicked()
{    
    ui->stackedWidget->setCurrentWidget(ui->HomePage);
    ui->btnBackHome->setVisible(false);
    ui->btnLoadGame->setEnabled(hasAnyValidSave());
}


void MainWindow::on_btnEvolveNormal_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->GamePage);
    ui->btnBackHome->setVisible(true);
    GameCanvas* canvas=ui->widgetCanvas;
    canvas->setFocus();
    canvas->reserGameCanvas();
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

void MainWindow::on_tableSaveList_cellDoubleClicked(int row, int column)
{
    int slot=row+1;
    if(!SaveManager::getInstance().slotHasSave(slot)){
        QMessageBox::information(this,"提示","该槽位无存档");
        return;
    }
    ui->widgetCanvas->loadGameBySlot(slot);
    ui->stackedWidget->setCurrentWidget(ui->GamePage);
}
bool MainWindow::hasAnyValidSave(){
    for(int i=1;i<=SaveManager::SLOT_COUNT;++i){
        if(SaveManager::getInstance().slotHasSave(i)){
            return true;
        }
    }
    return false;
}
void MainWindow::refreshSaveTable(){
    SaveManager& saveMgr=SaveManager::getInstance();
    QList<SaveBriefInfo> infoList=saveMgr.getBriefList();
    if(infoList.isEmpty())return;
    ui->tableSaveList->setRowCount(infoList.size());
    for(int row=0;row<infoList.size();++row){
        SaveBriefInfo info=infoList[row];
        QTableWidgetItem* item1=new QTableWidgetItem(info.saveTime);
        QTableWidgetItem* item2=new QTableWidgetItem(QString::number(info.eatTotal));
        QTableWidgetItem* item3=new QTableWidgetItem(QString::number(info.cellSize));
        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableSaveList->setItem(row,0,item1);
        ui->tableSaveList->setItem(row,1,item2);
        ui->tableSaveList->setItem(row,2,item3);
    }
}
void MainWindow::showSaveRightMenu(const QPoint &pos){
    QTableWidgetItem*item=ui->tableSaveList->itemAt(pos);
    if(!item)return;
    int row=item->row();
    m_currSelectSlot=row+1;
    if(!SaveManager::getInstance().slotHasSave(m_currSelectSlot))return;
    m_rightMenu->exec(ui->tableSaveList->viewport()->mapToGlobal(pos));
}
void MainWindow::deleteSelectedSave(){
    if((m_currSelectSlot<1)||(m_currSelectSlot>5)) return;
    QMessageBox::StandardButton ret=QMessageBox::question(this,"删除确认","确定要删除该存档吗？删除后无法恢复！"
                                                            ,QMessageBox::Yes|QMessageBox::No);
    if(ret!=QMessageBox::Yes)return;
    SaveManager::getInstance().deleteSlotSave(m_currSelectSlot);
    refreshSaveTable();
    ui->btnLoadGame->setEnabled(hasAnyValidSave());
    m_currSelectSlot=-1;
}