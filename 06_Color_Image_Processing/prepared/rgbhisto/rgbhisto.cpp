#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <stdexcept>

using namespace std;

const int INTENS_MIN = 0;
const int INTENS_MAX = 255;

enum Color {RED, GREEN, BLUE};

const QString file_name = "../lenna.png";

void calcHisto(QImage &image, double histo[], Color clr)
{
    for (int i = 0; i <= INTENS_MAX; i++)
    {
        histo[i] = 0;
    }
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        QRgb* ptr_row = (QRgb*)image.scanLine(indx_row);
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            int bean = 0;
            switch (clr)
            {
                case RED:
                    bean = qRed(ptr_row[indx_col]);
                    break;
                case GREEN:
                    bean = qGreen(ptr_row[indx_col]);
                    break;
                case BLUE:
                    bean = qBlue(ptr_row[indx_col]);
                    break;
                default:
                    throw runtime_error("Unknown color code");
            }
            histo[bean]++;
        }
    }
    int numb_pix = image.height() * image.width();
    for (int i = 0; i <= INTENS_MAX; i++)
    {
        histo[i] /= numb_pix;
    }
}//calcHisto

void printHisto(double histo[])
{
    for (int i = 0; i <= INTENS_MAX; i++)
    {
        QTextStream(stdout) << i << ": " << histo[i] << endl;   
    }
}//printHisto

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage image;
    QLabel label;
    if (image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << image.format() << endl;

        if (image.format() == QImage::Format_RGB32)
        {
            double rhisto[INTENS_MAX + 1];
            calcHisto(image, rhisto, RED);
            QTextStream(stdout) << "Red histogram: " << endl;  
            printHisto(rhisto);

            double ghisto[INTENS_MAX + 1];
            calcHisto(image, ghisto, GREEN);
            QTextStream(stdout) << "Green histogram: " << endl;  
            printHisto(ghisto);

            double bhisto[INTENS_MAX + 1];
            calcHisto(image, bhisto, GREEN);
            QTextStream(stdout) << "Blue histogram: " << endl;  
            printHisto(bhisto);
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
