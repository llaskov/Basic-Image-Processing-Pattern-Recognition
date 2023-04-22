#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTextStream>
#include <QString>

struct PCoord
{
    int x;
    int y;
};

void printNeighbor(const QImage& image, const PCoord& pix)
{
    for (int indx_row = pix.x - 1; indx_row <= pix.x + 1; indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() + indx_row * image.bytesPerLine());
        for (int indx_col = pix.y - 1; indx_col <= pix.y + 1; indx_col++)
        {
            QTextStream(stdout) << ptr_row[indx_col] << " ";   
        }
        QTextStream(stdout) << "\n";
    }    
}//printNeighbor

int main(int argc, char* argv[])
{
    const QString file_name = "gray_lenna.png";

    QApplication app(argc, argv);
    QImage image;
    QLabel label;
    if (image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << image.format() << endl;

        if (image.format() == QImage::Format_Grayscale8)
        {
            PCoord center;
            center.x = image.height() / 2;
            center.y = image.width() / 2;
            printNeighbor(image, center);    
        } 
             
        label.setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main
