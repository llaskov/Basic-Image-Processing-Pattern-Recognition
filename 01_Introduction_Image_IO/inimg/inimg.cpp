#include <QtGui>
#include <QApplication>
#include <QTextStream>

int main()
{

    QImage image;
    if (image.load("in.png"))
    {
        QTextStream(stdout) << "Height: " << image.height() << endl; 
        QTextStream(stdout) << "Width: " << image.width() << endl;
        for (int indx_row = 0; indx_row < image.height(); indx_row++)
        {
            QRgb* pnt_row = (QRgb*)image.constScanLine(indx_row);
            for (int indx_col = 0; indx_col < image.width(); indx_col++)
            {
                QRgb pixel = pnt_row[indx_col];
                int red = qRed(pixel);
                int grn = qGreen(pixel);
                int blu = qBlue(pixel);
                QTextStream(stdout) << "(" << red << ", " << grn << ", " << blu << ") ";                   
            }
            QTextStream(stdout) << "\n";
        }  
    }
    else
    {
        QTextStream(stdout) << "Cannot load image." << endl;
    }

    return 0;   
}//main
