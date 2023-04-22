#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <vector>
#include <complex>
#include <cmath>

using namespace std;

const int INTENS_MIN = 0;
const int INTENS_MAX = 255;

const QString file_name = "../gray_lenna_small.png";

void dft2d(
        const QImage &image,                        // input image 
        vector< vector< complex<double> > > &trs    // Fourier transform of the input
        )
{
    // dimensions
    int m = image.height();     // number of rows
    int n = image.width();      // number of columns

    // resize FT vector
    trs.resize(m);
    for (int indx_row = 0; indx_row < m; indx_row++)
    {
        trs[indx_row].resize(n);
    }

    for (int indx_trs_row = 0; indx_trs_row < m; indx_trs_row++)
    {
        for (int indx_trs_col = 0; indx_trs_col < n; indx_trs_col++)
        {
            complex<double> sum(0, 0);
            for (int indx_img_row = 0; indx_img_row < m; indx_img_row++)
            {
                quint8* ptr_row = 
                    (quint8*)(image.bits() + indx_img_row * image.bytesPerLine());
                for (int indx_img_col = 0; indx_img_col < n; indx_img_col++)
                {
                    double expnt = 
                        -2 * M_PI * (
                                1.0 * indx_trs_row * indx_img_row / m 
                                + 1.0 * indx_trs_col * indx_img_col / n
                                );
                    double centered = ptr_row[indx_img_col] * pow(-1.0, indx_img_row + indx_img_col);
                    sum += centered * exp(complex<double>(0, expnt));
                }
            }
            trs[indx_trs_row][indx_trs_col] = sum;
        }
    }
}//dft2d

void spectrum(
        const vector< vector< complex<double> > > &trs,   // Fourier transform of the input
        QImage &spc_img
        )
{
    // dimensions
    int m = spc_img.height();       // number of rows
    int n = spc_img.width();        // number of columns

    vector< vector< int > > spc_vct;
    spc_vct.resize(m);
    for (int indx_row = 0; indx_row < m; indx_row++)
    {
        spc_vct[indx_row].resize(n);
    }

    int min = 0;
    int max = 0;
    for (int indx_row = 0; indx_row < m; indx_row++)
    {
        for (int indx_col = 0; indx_col < n; indx_col++)
        {
            spc_vct[indx_row][indx_col] = (int)log(1 + abs(trs[indx_row][indx_col]));
            if (spc_vct[indx_row][indx_col] < min)
            {
                min = spc_vct[indx_row][indx_col];
            }
            if (spc_vct[indx_row][indx_col] > max)
            {
                max = spc_vct[indx_row][indx_col];
            }
        }
    }

    // scale
    int diff = max - min;
    for (int indx_row = 0; indx_row < m; indx_row++)
    {
        for (int indx_col = 0; indx_col < n; indx_col++)
        {
            spc_vct[indx_row][indx_col] = (spc_vct[indx_row][indx_col] - min) * INTENS_MAX / (diff);   
        }
    }

    for (int indx_row = 0; indx_row < m; indx_row++)
    {
        quint8* ptr_row = 
            (quint8*)(spc_img.bits() + indx_row * spc_img.bytesPerLine());
        for (int indx_col = 0; indx_col < n; indx_col++)
        {
            ptr_row[indx_col] = spc_vct[indx_row][indx_col];
        }
    }
}//spectrum

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage image;
    QLabel label;
    if (image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << image.format() << endl;

        QImage spc(image.height(), image.width(), QImage::Format_Grayscale8);
        if (image.format() == QImage::Format_Grayscale8)
        {
            vector< vector< complex<double> > > trs;
            dft2d(image, trs);
            spectrum(trs, spc);
        } 
             
        label.setPixmap(QPixmap::fromImage(spc));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main


