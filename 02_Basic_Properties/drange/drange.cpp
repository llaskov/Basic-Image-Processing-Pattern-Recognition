#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTextStream>
#include <QString>

void toGray(QImage &image)
{
    const double RED_COEF = 0.2989;
    const double GRN_COEF = 0.5870;
    const double BLU_COEF = 0.1140; 
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        QRgb* pnt_row = (QRgb*)image.scanLine(indx_row);
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            int red = qRed(pnt_row[indx_col]);
            int grn = qGreen(pnt_row[indx_col]);
            int blu = qBlue(pnt_row[indx_col]);
            int grey = RED_COEF * red + GRN_COEF * grn + BLU_COEF * blu;
            pnt_row[indx_col] = qRgb(grey, grey, grey);
                               
        }
    }
}//toGray

struct DRange
{
    int min;
    int max;
};

DRange dynamicRange(QImage &image)
{
    DRange d_range;
    d_range.min = 0;
    d_range.max = 0;
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() + indx_row * image.bytesPerLine());
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
            if (ptr_row[indx_col] < d_range.min)
            {
                d_range.min = ptr_row[indx_col];
            }
            if (ptr_row[indx_col] > d_range.max)   
            {
                d_range.max = ptr_row[indx_col];        
            }     
        }
    }

    return d_range;
}//dynamicRange

int main(int argc, char* argv[])
{
    const QString file_name = "digit.png";

    QApplication app(argc, argv);
    QImage color_image;
    QLabel label;
    if (color_image.load(file_name))
    {
        QTextStream(stdout) << "Image loaded: " << file_name << endl;
        QTextStream(stdout) << "Format: " << color_image.format() << endl;

        //if (color_image.format() == QImage::Format_RGB32)
        {
            toGray(color_image);
            if (color_image.allGray())
            {
                QImage gray_image = color_image.convertToFormat(QImage::Format_Grayscale8);
                DRange d_range = dynamicRange(gray_image);
                QTextStream(stdout) << "Dynamic range: " << d_range.min << " " << d_range.max << endl;
                QTextStream(stdout) << "Contrast: " << d_range.max - d_range.min << endl;

                gray_image.save("gray_" + file_name);
            }
        } 
             
        label.setPixmap(QPixmap::fromImage(color_image));
    }
    else
    {
        QTextStream(stdout) << "Cannot load image: " << file_name << endl;
    }

    label.show();

    return app.exec();
}//main
