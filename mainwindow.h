#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QPushButton;
class myScene;
class QStandardItemModel;
class QTableView;
class QFrame;
class MarkerImageView;
class ControllerTableView;
class QPushButton;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    void initModel();
    bool loadImage(const QString& fileName);
    void createActions();
    void createMenus();
    void createToolBars();
    QFrame* createControlPanel();
private slots:
    void openImage();
    void getOnScreenToggled(bool toggle);
    void markerInsertedOnScreen();
private:

    QStandardItemModel *model;
    ControllerTableView *tableView;
    MarkerImageView *markerImageView;
    QAction *openAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomRestoreAct;
    QPushButton *placeButton;
};

#endif // MAINWINDOW_H
