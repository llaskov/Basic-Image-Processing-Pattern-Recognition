#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

const int KERN_DIM = 3;

const QString file_name = "../gray_lenna.png";

void ameanFlt(const QImage &in_image, QImage &out_image, int kern_dim = KERN_DIM)
{
    const int DK = kern_dim / 2;
    const double coef = 1.0 / (kern_dim * kern_dim);
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        quint8* ptr_out_row = (quint8*)(out_image.bits() 
                + indx_row * out_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            double conv_value = 0;
            for (int indx_ker_row = 0; indx_ker_row < kern_dim; indx_ker_row++)
            {
                int x = indx_row - DK + indx_ker_row;
                if (x >= 0 && x < in_image.height())
                {
                    quint8* ptr_in_row = (quint8*)(in_image.bits() 
                        + x * in_image.bytesPerLine());
                    for (int indx_ker_col = 0; indx_ker_col < kern_dim; indx_ker_col++)
                    {                    
                        int y = indx_col - DK + indx_ker_col;
                        if (y >= 0 && y < in_image.width())
                        {
                            conv_value += ptr_in_row[y] * coef;
                        } 
                    }
                }
            }
            ptr_out_row[indx_col] = (int)conv_value;  
        }
    }   
}//aeanFlt

void gmeanFlt(const QImage &in_image, QImage &out_image, int kern_dim = KERN_DIM)
{
    const int DK = kern_dim / 2;
    const double coef = 1.0 / (kern_dim * kern_dim);
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        quint8* ptr_out_row = (quint8*)(out_image.bits() 
                + indx_row * out_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            double prod = 1;
            for (int indx_ker_row = 0; indx_ker_row < kern_dim; indx_ker_row++)
            {
                int x = indx_row - DK + indx_ker_row;
                if (x >= 0 && x < in_image.height())
                {
                    quint8* ptr_in_row = (quint8*)(in_image.bits() 
                        + x * in_image.bytesPerLine());
                    for (int indx_ker_col = 0; indx_ker_col < kern_dim; indx_ker_col++)
                    {                    
                        int y = indx_col - DK + indx_ker_col;
                        if (y >= 0 && y < in_image.width())
                        {
                            prod *= ptr_in_row[y];
                        } 
                    }
                }
            }
            ptr_out_row[indx_col] = (int)pow(prod, coef);  
        }
    }   
}//gmeanFlt

void harmFlt(const QImage &in_image, QImage &out_image, int kern_dim = KERN_DIM)
{
    const int DK = kern_dim / 2;
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        quint8* ptr_out_row = (quint8*)(out_image.bits() 
                + indx_row * out_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            double sum = 0;
            for (int indx_ker_row = 0; indx_ker_row < kern_dim; indx_ker_row++)
            {
                int x = indx_row - DK + indx_ker_row;
                if (x >= 0 && x < in_image.height())
                {
                    quint8* ptr_in_row = (quint8*)(in_image.bits() 
                        + x * in_image.bytesPerLine());
                    for (int indx_ker_col = 0; indx_ker_col < kern_dim; indx_ker_col++)
                    {                    
                        int y = indx_col - DK + indx_ker_col;
                        if (y >= 0 && y < in_image.width())
                        {
                            sum += 1.0 / ptr_in_row[y];
                        } 
                    }
                }
            }
            ptr_out_row[indx_col] = (int)((kern_dim * kern_dim) / sum);  
        }
    }   
}//harmFlt

int median(vector<int> &intens)
{
    sort(intens.begin(), intens.end());
    intens.erase(unique(intens.begin(), intens.end()), intens.end());

    return intens[intens.size() / 2];
}//median

void medianFlt(const QImage &in_image, QImage &out_image, int kern_dim = KERN_DIM)
{
    const int DK = kern_dim / 2;
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        quint8* ptr_out_row = (quint8*)(out_image.bits() 
                + indx_row * out_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            vector<int> intens;
            for (int indx_ker_row = 0; indx_ker_row < kern_dim; indx_ker_row++)
            {
                int x = indx_row - DK + indx_ker_row;
                if (x >= 0 && x < in_image.height())
                {
                    quint8* ptr_in_row = (quint8*)(in_image.bits() 
                        + x * in_image.bytesPerLine());
                    for (int indx_ker_col = 0; indx_ker_col < kern_dim; indx_ker_col++)
                    {                    
                        int y = indx_col - DK + indx_ker_col;
                        if (y >= 0 && y < in_image.width())
                        {
                            intens.push_back(ptr_in_row[y]);
                        } 
                    }
                }
            }
            ptr_out_row[indx_col] = median(intens);  
        }
    }   
}//medianFlt

void filterSelect(const QImage &in_image, QImage &out_image, const QString &filter_name)
{
    if (filter_name == "amean")
    {
        ameanFlt(in_image, out_image);
    }
    else if (filter_name == "gmean")
    {
         gmeanFlt(in_image, out_image, 5);
    }
    else if (filter_name == "harmonic")
    {
         harmFlt(in_image, out_image, 15);
    }
    else if (filter_name == "median")
    {
         medianFlt(in_image, out_image, 5);
    }
    else
    {
        throw runtime_error("filterSelect(): unknown filter name.");
    }
}//filterSelect

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QImage in_image;
    QLabel label;

    if (in_image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << in_image.format() << endl;

        QImage out_image(in_image.width(), in_image.height(), QImage::Format_Grayscale8);

        if (in_image.format() == QImage::Format_Grayscale8)
        {
            filterSelect(in_image, out_image, "median");   
        } 
             
        label.setPixmap(QPixmap::fromImage(out_image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main

