#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

void dft1d(
        const vector< complex<double> >& input, 
        vector< complex<double> >& output,
        int dir
        )
{
    for (int indx_out = 0; indx_out < input.size(); indx_out++)
    {
        complex<double> sum(0, 0);
        for (int indx_in = 0; indx_in < input.size(); indx_in++)
        {
            double expnt = 2 * M_PI * indx_in * indx_out / input.size();
            expnt *= (dir ? 1 : -1);
            sum += input[indx_in] * exp(complex<double>(0, expnt));
        }
        sum /= (dir ? input.size() : 1);
        output.push_back(sum);
    }
}//dft1d

void print(const vector< complex<double> >& vect)
{
    for (int indx_v = 0; indx_v < vect.size(); indx_v++)
    {
        cout << vect[indx_v] << " ";
    }
    cout << endl;
}//print

int main()
{
    const int SIZE = 256;

    vector< complex<double> > sig;

    double delta = 2 * M_PI / SIZE;
    for (int indx_sig = 0; indx_sig < SIZE; indx_sig++)
    {
        double x = indx_sig * delta;
        complex<double> sample(sin(x), 0);
        sig.push_back(sample);
    }
    cout << "Signal: " << endl;
    print(sig);
    vector< complex<double> > trs;
    dft1d(sig, trs, 0);
    cout << "Transformed: " << endl;
    print(trs);
    dft1d(trs, sig, 1);
    cout << "Signal: " << endl;
    print(sig);

    return 0;
}//main
