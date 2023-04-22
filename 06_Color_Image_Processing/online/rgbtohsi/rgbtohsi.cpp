#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <cmath>

using namespace std;

const QString file_name = "../lenna.png";

const int INTENS_MAX = 255;
const double EPS = 1.0e-8;

void split(
        const QImage &in_image,
        QImage &red_image,
        QImage &grn_image,
        QImage &blu_image
        )
{
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        // color
        QRgb* ptr_row_input = (QRgb*)in_image.scanLine(indx_row);

        // red
        quint8* ptr_row_red = (quint8*)(red_image.bits() 
                + indx_row * red_image.bytesPerLine());

        // green 
        quint8* ptr_row_grn = (quint8*)(grn_image.bits() 
                + indx_row * grn_image.bytesPerLine());

        // blue
        quint8* ptr_row_blu = (quint8*)(blu_image.bits() 
                + indx_row * blu_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            int red = qRed(ptr_row_input[indx_col]);
            int grn = qGreen(ptr_row_input[indx_col]);
            int blu = qBlue(ptr_row_input[indx_col]);

            ptr_row_red[indx_col] = red;
            ptr_row_grn[indx_col] = grn;
            ptr_row_blu[indx_col] = blu;
        }
    }    
}//split

double hue(double red, double grn, double blu)
{
    red /= INTENS_MAX;
    grn /= INTENS_MAX;
    blu /= INTENS_MAX;

    double num = ((red - grn) + (red - blu)) / 2.0;
    double den = sqrt((red - grn) * (red - grn) + (red - blu) * (grn - blu));
    double phi = acos(num / (den + EPS));

    return blu <= grn ? phi : 360 - phi; 
}//hue

inline double min(double x, double y)
{
    return x < y ? x : y;
}//min

double sat(double red, double grn, double blu)
{
    red /= INTENS_MAX;
    grn /= INTENS_MAX;
    blu /= INTENS_MAX;

    return 1 - (3.0 * min(min(red, grn), blu)) / (red + grn + blu + EPS);    
}//sat

void rgbToHsi(
        const QImage &in_image,
        QImage &hue_image,
        QImage &sat_image,
        QImage &int_image
        )
{
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        // color
        QRgb* ptr_row_input = (QRgb*)in_image.scanLine(indx_row);

        // red
        quint8* ptr_row_hue = (quint8*)(hue_image.bits() 
                + indx_row * hue_image.bytesPerLine());

        // green 
        quint8* ptr_row_sat = (quint8*)(sat_image.bits() 
                + indx_row * sat_image.bytesPerLine());

        // blue
        quint8* ptr_row_int = (quint8*)(int_image.bits() 
                + indx_row * int_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            int red = qRed(ptr_row_input[indx_col]);
            int grn = qGreen(ptr_row_input[indx_col]);
            int blu = qBlue(ptr_row_input[indx_col]);

            ptr_row_hue[indx_col] = (int) (hue(red, grn, blu));
            ptr_row_sat[indx_col] = (int) (sat(red, grn, blu) * 100);
            ptr_row_int[indx_col] = (red + grn + blu) / 3;
        }
    }    
}//rgbToHsi

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage in_image;

    QLabel frst_label;
    QLabel scnd_label;
    QLabel thrd_label;

    if (in_image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << in_image.format() << endl;

        QImage frst_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);
        QImage scnd_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);
        QImage thrd_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);

        if (in_image.format() == QImage::Format_RGB32)
        {
            // split into red, green, and blue channels
            //split(in_image, frst_image, scnd_image, thrd_image);
            
            // to HSI
            rgbToHsi(in_image, frst_image, scnd_image, thrd_image);
        }

        frst_label.setPixmap(QPixmap::fromImage(frst_image));
        scnd_label.setPixmap(QPixmap::fromImage(scnd_image));
        thrd_label.setPixmap(QPixmap::fromImage(thrd_image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    frst_label.show();
    scnd_label.show();
    thrd_label.show();

    return app.exec();
}//main
