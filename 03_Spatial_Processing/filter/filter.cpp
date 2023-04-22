#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

const int INTENS_MIN = 0;
const int INTENS_MAX = 255;
const int KERN_SIZE = 3;
const double KERN_ELEM = 1.0 / (KERN_SIZE * KERN_SIZE);

double kernel[][KERN_SIZE] = {
    {KERN_ELEM, KERN_ELEM, KERN_ELEM},
    {KERN_ELEM, KERN_ELEM, KERN_ELEM},
    {KERN_ELEM, KERN_ELEM, KERN_ELEM}
};

double kernel_lap[][KERN_SIZE] = {
    {1, 1, 1},
    {1, -8, 1},
    {1, 1, 1}
};

const QString file_name = "../gray_lenna.png";

void filter(const QImage &in_image, QImage &out_image, double kernel[][KERN_SIZE])
{
    const int DK = KERN_SIZE / 2;
    for (int indx_row = 0; indx_row < in_image.height(); indx_row++)
    {
        quint8* ptr_out_row = (quint8*)(out_image.bits() 
                + indx_row * out_image.bytesPerLine());
        for (int indx_col = 0; indx_col < in_image.width(); indx_col++)
        {
            double conv_value = 0;
            for (int indx_ker_row = 0; indx_ker_row < KERN_SIZE; indx_ker_row++)
            {
                int x = indx_row - DK + indx_ker_row;
                if (x >= 0 && x < in_image.height())
                {
                    quint8* ptr_in_row = (quint8*)(in_image.bits() 
                        + x * in_image.bytesPerLine());
                    for (int indx_ker_col = 0; indx_ker_col < KERN_SIZE; indx_ker_col++)
                    {                    
                        int y = indx_col - DK + indx_ker_col;
                        if (y >= 0 && y < in_image.width())
                        {
                            conv_value += ptr_in_row[y] * kernel[indx_ker_row][indx_ker_col];
                        } 
                    }
                }
            }
            ptr_out_row[indx_col] = (int)conv_value;  
        }
    }    
}//filter

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
            filter(in_image, out_image, kernel_lap);   
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


