#include <QtGui>
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QImage image;
    QLabel label;
    if (image.load("lenna.png"))
    {
        label.setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        label.setText("Cannot load image.");
    }

    label.show();

    return app.exec();   
}//main
