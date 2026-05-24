#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMenu>
#include "Utils/savemanager.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_btnStartGame_clicked();

    void on_btnLoadGame_clicked();

    void on_btnBook_clicked();

    void on_btnSetting_clicked();

    void on_btnExit_clicked();

    void on_btnBackHome_clicked();

    void on_btnEvolveNormal_clicked();

    void on_btnEvolveEndless_clicked();

    void on_btnTrail_clicked();

    void deleteSelectedSave();

    void on_tableSaveList_cellDoubleClicked(int row, int column);
    void showSaveRightMenu(const QPoint& pos);
    void refreshSaveTable();
    bool hasAnyValidSave();

private:
    Ui::MainWindow *ui;
    QMenu *m_rightMenu;
    QString m_currSelectFile;
};
#endif // MAINWINDOW_H
