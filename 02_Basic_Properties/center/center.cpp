#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTextStream>
#include <QString>

struct PCoord
{
    int row;
    int col;
};

PCoord center(QImage& image)
{
    const int RECT_SIZE = 20;

    QPainter painter(&image);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::green);
    PCoord cnt;
    cnt.row = image.height() / 2;
    cnt.col = image.width() / 2;
    painter.drawRect(cnt.row, cnt.col, RECT_SIZE, RECT_SIZE);
    painter.end();

    return cnt;
}//center   

int main(int argc, char* argv[])
{
    const QString file_name = "lenna.png";

    QApplication app(argc, argv);
    QImage image;
    QLabel label;
    if (image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        PCoord cnt = center(image);
        QTextStream(stdout) << "(" << cnt.row << ", " << cnt.col << ")" << endl;
        label.setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main
