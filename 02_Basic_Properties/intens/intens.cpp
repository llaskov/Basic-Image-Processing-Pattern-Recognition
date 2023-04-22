#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTextStream>
#include <QString>

void intensResample(QImage& image)
{
    const int I1 = 64;
    const int I2 = 128;
    const int I3 = 192;

    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() + indx_row * image.bytesPerLine());
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            if (ptr_row[indx_col] < I1)
            {
                ptr_row[indx_col] = I1 / 2;
            }
            else if (ptr_row[indx_col] < I2)
            {
                ptr_row[indx_col] = I1 + I1 / 2;
            }
            else if (ptr_row[indx_col] < I3)
            {
                ptr_row[indx_col] = I2 + I1 / 2;
            }
            else
            {
                ptr_row[indx_col] = I3 + I1 / 2;
            }
        }
    }
}//intensResample

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
            intensResample(image);   
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
