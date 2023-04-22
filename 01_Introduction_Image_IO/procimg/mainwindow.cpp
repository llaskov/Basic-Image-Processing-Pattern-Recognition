#include <QtWidgets>

#include "mainwindow.h"

// default constructor
MainWindow::MainWindow() //: ptr_label(new QLabel)
{
    ptr_image = new QImage;
    ptr_label = new QLabel;
    setCentralWidget(ptr_label);
    createActions();
}//MainWindow

// destructor
MainWindow::~MainWindow()
{
    if (ptr_label)
    {
        delete ptr_label;
        ptr_label = nullptr;
    }
    if (ptr_image)
    {
        delete ptr_image;
        ptr_image = nullptr;
    }
}//~MainWindow

// open an image file
void MainWindow::open()
{
    //if (maybeSave()) 
    {
        QString file_name = QFileDialog::getOpenFileName(this);
        if (!file_name.isEmpty())
        {
            loadFile(file_name);
            ptr_label->setPixmap(QPixmap::fromImage(*ptr_image));
            ptr_label->show();
        }
    }
}//open

void MainWindow::loadFile(const QString &file_name)
{
    if (!ptr_image->load(file_name))
    {
        QMessageBox::warning(
                this, 
                tr("Image processor"),
                tr("Cannot read file %1").arg(file_name)
        );
    }
}//loadFile

// create actions
void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
}//createActions
