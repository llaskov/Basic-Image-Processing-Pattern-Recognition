#include <iostream>

using namespace std;

#include <gsl/gsl_sf_bessel.h>

int main()
{
    double x = 15.0;
    double y = gsl_sf_bessel_J0 (x);

    cout << "J0(" << x << ") = " << y << endl;

    return 0;
}
