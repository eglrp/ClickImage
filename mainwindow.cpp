#include "mainwindow.h"
#include "controllertableview.h"
#include "markerimageview.h"
#include <QtWidgets>
#include <QDebug>
#include "mytest.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    model= new QStandardItemModel(this);
    tableView=new ControllerTableView(this);
    tableView->setModel(model);
    markerImageView= new MarkerImageView;
    markerImageView->setModel(model);
    markerImageView->setSelectionModel(tableView->selectionModel());
    connect(markerImageView,SIGNAL(markerInsertedOnScreen()),this,SLOT(markerInsertedOnScreen()));

    createActions();
    createMenus();
    createToolBars();



    QFrame* controlPanel=createControlPanel();
    markerImageView->setMinimumSize(400,400);
    QHBoxLayout *thisLayout=new QHBoxLayout;
    thisLayout->addWidget(markerImageView);
    thisLayout->addWidget(controlPanel);
    QWidget *cWidget=new QWidget();
    cWidget->setLayout(thisLayout);
    setCentralWidget(cWidget);

    initModel();
}

void MainWindow::initModel()
{
    QList<QStandardItem*> list1,list2;
    list1.append(new QStandardItem("100"));
    list1.append(new QStandardItem("100"));
    list2.append(new QStandardItem("150"));
    list2.append(new QStandardItem("200"));
    model->appendRow(list1);
    model->appendRow(list2);
}

bool MainWindow::loadImage(const QString &fileName)
{
    QImage image(fileName);
    if(image.isNull()){
        QMessageBox::information(this,"Error",tr("Cannot load %1").arg(fileName));
        return false;
    }
    markerImageView->loadImage(image);
    return true;
}

void MainWindow::createMenus()
{
    QMenu *menu=menuBar()->addMenu(tr("&File"));
    menu->addAction(openAct);

    menu=menuBar()->addMenu(tr("&View"));
    menu->addAction(zoomInAct);
    menu->addAction(zoomOutAct);
    menu->addAction(zoomRestoreAct);
}

void MainWindow::createToolBars()
{
    QToolBar *toolbar=addToolBar(tr("File"));
    toolbar->addAction(openAct);

    QToolBar *viewToolBar=addToolBar(tr("View"));
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);
    viewToolBar->addAction(zoomRestoreAct);

}

QFrame *MainWindow::createControlPanel()
{
    QFrame* frame=new QFrame;

    QPushButton *addButton=new QPushButton(tr("+"));
    QPushButton *removeButton=new QPushButton(tr("-"));
    QPushButton *upButton=new QPushButton(tr("^"));
    QPushButton *downButton=new QPushButton(tr("V"));
    placeButton=new QPushButton(tr("pickOnScreen"));
    placeButton->setCheckable(true);

    QSizePolicy policy1=addButton->sizePolicy();
    policy1.setHorizontalPolicy(QSizePolicy::Fixed);
    policy1.setVerticalPolicy(QSizePolicy::Fixed);
    QSizePolicy policy2=policy1;

    policy1.setHorizontalStretch(1);
    policy2.setHorizontalStretch(5);
    addButton->setSizePolicy(policy1);
    removeButton->setSizePolicy(policy1);
    upButton->setSizePolicy(policy1);
    addButton->setMinimumWidth(10);

    downButton->setMinimumWidth(10);
    removeButton->setMinimumWidth(10);
    upButton->setMinimumWidth(10);
    qDebug()<<removeButton->minimumSizeHint();
    downButton->setSizePolicy(policy1);
    placeButton->setSizePolicy(policy2);


    connect(addButton,SIGNAL(clicked()),tableView,SLOT(addOneItem()));
    connect(removeButton,SIGNAL(clicked()),tableView,SLOT(removeOneItem()));
    connect(placeButton,SIGNAL(toggled(bool)),this,SLOT(getOnScreenToggled(bool)));
    connect(upButton,SIGNAL(clicked(bool)),tableView,SLOT(moveUp()));
    connect(downButton,SIGNAL(clicked(bool)),tableView,SLOT(moveDown()));
    QHBoxLayout *buttonsLayout=new QHBoxLayout;
    buttonsLayout->addWidget(placeButton);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addWidget(upButton);
    buttonsLayout->addWidget(downButton);
    QVBoxLayout *thisLayout=new QVBoxLayout;
    thisLayout->addLayout(buttonsLayout);
    thisLayout->addWidget(tableView);
    frame->setLayout(thisLayout);
    return frame;
}


void MainWindow::createActions()
{
    openAct=new QAction(tr("&Open"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(openImage()));

    zoomInAct=new QAction(tr("ZoomIn"),this);
    connect(zoomInAct,SIGNAL(triggered()),markerImageView,SLOT(zoomIn()));

    zoomOutAct=new QAction(tr("ZoomOut"),this);
    connect(zoomOutAct,SIGNAL(triggered()),markerImageView,SLOT(zoomOut()));

    zoomRestoreAct=new QAction(tr("Restore View"),this);
    connect(zoomRestoreAct,SIGNAL(triggered()),markerImageView,SLOT(zoomRestore()));

}


void MainWindow::openImage()
{
    QFileDialog dialog(this,tr("Open Image"),QDir::currentPath());
    QStringList formatList;
    formatList.append("image/jpeg");
    formatList.append("image/png");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(formatList);
    while(dialog.exec() == QDialog::Accepted){
        if(loadImage(dialog.selectedFiles().first())) break;
    }

}


void MainWindow::getOnScreenToggled(bool toggle)
{
    if(toggle){
        markerImageView->setMode(MarkerImageView::InsertItem);
    }else{
        markerImageView->setMode(MarkerImageView::MoveItem);
    }
}

void MainWindow::markerInsertedOnScreen()
{
    placeButton->setChecked(false);
}
