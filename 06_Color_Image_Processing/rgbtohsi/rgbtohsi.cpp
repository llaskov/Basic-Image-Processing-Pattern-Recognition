#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <cmath>

using namespace std;

const QString file_name = "../lenna.png";

const double EPS = 1.0e-8;

inline void norm(double &red, double &grn, double &blu)
{
    /// !!!!!!!!!! wrong
    double sum = red + grn + blu;
    red /= sum;
    grn /= sum;
    blu /= sum;
}//norm

double hue(double red, double grn, double blu)
{
    norm(red, grn, blu);
    double num = ((red - grn) + (red - blu)) / 2.0;
    double den = sqrt((red - grn) * (red - grn) + (red - blu) * (grn - blu));
    double phi = acos(num / (den + EPS));

    QTextStream(stdout) << (blu <= grn ? phi : 360 - phi) << endl;

    return blu <= grn ? phi : 360 - phi;
}//hue

void rgbToHsi(
        const QImage &in_image,
        QImage &h_image,
        QImage &s_image,
        QImage &i_image
        )
{
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        QRgb* ptr_row_input = (QRgb*)in_image.scanLine(indx_row);
        
        quint8* ptr_row_hue = (quint8*)(h_image.bits() 
                + indx_row * h_image.bytesPerLine());
        quint8* ptr_row_sat = (quint8*)(s_image.bits() 
                + indx_row * s_image.bytesPerLine());
        quint8* ptr_row_int = (quint8*)(i_image.bits() 
                + indx_row * i_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            int red = qRed(ptr_row_input[indx_col]);
            int grn = qGreen(ptr_row_input[indx_col]);
            int blu = qBlue(ptr_row_input[indx_col]);

            ptr_row_hue[indx_col] = (int)(hue(red, grn, blu));
            ptr_row_sat[indx_col] = grn;
            ptr_row_int[indx_col] = blu;
        }
    }
}//rgbToHsi

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage in_image;
    QLabel h_label;
    QLabel s_label;
    QLabel i_label;
    if (in_image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << in_image.format() << endl;

        QImage h_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);
        QImage s_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);
        QImage i_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);

        if (in_image.format() == QImage::Format_RGB32)
        {
            rgbToHsi(in_image, h_image, s_image, i_image);
        } 
             
        h_label.setPixmap(QPixmap::fromImage(h_image));
        s_label.setPixmap(QPixmap::fromImage(s_image));
        i_label.setPixmap(QPixmap::fromImage(i_image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    h_label.show();
    //s_label.show();
    //i_label.show();

    return app.exec();
}//main
