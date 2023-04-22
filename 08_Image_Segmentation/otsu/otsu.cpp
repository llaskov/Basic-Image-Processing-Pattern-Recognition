#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <cmath>

using namespace std;

const int INTENS_MIN = 0;
const int INTENS_MAX = 255;

const double EPS = 1.0e-14;

const QString file_name = "../gray_lenna.png";

void calcHisto(QImage &image, double histo[])
{
    for (int i = 0; i <= INTENS_MAX; i++)
    {
        histo[i] = 0;
    }
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() 
                + indx_row * image.bytesPerLine());
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            histo[ptr_row[indx_col]]++;
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

void thresh(QImage &image, int thr)
{
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() 
                + indx_row * image.bytesPerLine());
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            ptr_row[indx_col] = 
                (ptr_row[indx_col] < thr) ? INTENS_MIN : INTENS_MAX;
        }
    }
}//thresh

int otsu(const double histo[])
{
    // compute cumulative sums
    double p_1[INTENS_MAX + 1] = {0};
    p_1[0] = histo[0];
    for (int i = 1; i <= INTENS_MAX; i++)
    {
        p_1[i] = p_1[i - 1] + histo[i];
    }

    // cumulative mean
    double m[INTENS_MAX + 1] = {0};
    for (int i = 1; i <= INTENS_MAX; i++)
    {
        m[i] = m[i - 1] + i * histo[i];
    }

    // global mean
    double m_g = m[INTENS_MAX];

    // between-class
    double b_c[INTENS_MAX + 1] = {0};
    for (int i = 1; i <= INTENS_MAX; i++)
    {
        double div = (p_1[i] * (1 - p_1[i]));
        b_c[i] = 
            fabs(div < EPS) ? 0 :
            ((m_g * p_1[i] - m[i]) * (m_g * p_1[i] - m[i])) / div;
    }

    // find max
    double max = 0;
    int max_i = 0;
    for (int i = 0; i <= INTENS_MAX; i++)
    {
        if (b_c[i] > max)
        {
            max = b_c[i];
            max_i = i;
        }
    }

    QTextStream(stdout) << "Th: " << max_i << endl;

    return max_i;
}//otsu

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage image;
    QLabel label;
    if (image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << image.format() << endl;

        if (image.format() == QImage::Format_Grayscale8)
        {
            double histo[INTENS_MAX + 1];
            calcHisto(image, histo);  
            int th = otsu(histo);
            thresh(image, th);
        } 
             
        label.setPixmap(QPixmap::fromImage(image));
        image.save("out.png");
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main


