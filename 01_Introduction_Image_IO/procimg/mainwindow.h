#ifndef MAINWINDOW_H
#define MINWINFDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();
    void loadFile(const QString &fileName);

protected:

private slots:
    void open();

private:
    QString curr_file_name;
    QImage* ptr_image;
    QLabel* ptr_label;

    void createActions();
};

#endif
