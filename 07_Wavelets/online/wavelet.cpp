#include <iostream>
#include <cmath>

using namespace std;

#include <gsl/gsl_sort.h>
#include <gsl/gsl_wavelet.h>

const int DATA_SIZE = 256;
const int NC = 20;

void generateSignal(double* signal)
{
    double delta = 2 * M_PI / DATA_SIZE;
    for (int indx_sig = 0; indx_sig < DATA_SIZE; indx_sig++)
    {
        double x = indx_sig * delta;
        signal[indx_sig] = sin(x);
    }
}//generateSignal

void print(const double* signal)
{
    for (int indx_sig = 0; indx_sig < DATA_SIZE; indx_sig++)
    {
        cout << signal[indx_sig] << " ";
    }
    cout << endl;
}//print

void getAbs(const double* signal, double* abscoeff)
{
    for (int indx_sig = 0; indx_sig < DATA_SIZE; indx_sig++)
    {
        abscoeff[indx_sig] = fabs(signal[indx_sig]);
    }
}//getAbs

int main()
{
    double* input = nullptr;
    input = new double[DATA_SIZE];

    // generate the input signal
    generateSignal(input);

    // print
    cout << "Input signal: " << endl;
    print(input);

    // allocate wavelets and workspace
    gsl_wavelet* w = gsl_wavelet_alloc(gsl_wavelet_daubechies, 4);
    gsl_wavelet_workspace* work = gsl_wavelet_workspace_alloc(DATA_SIZE);

    gsl_wavelet_transform_forward(w, input, 1, DATA_SIZE, work);

    // print
    cout << "Transformed: " << endl;
    print(input);

    double* abscoeff = new double[DATA_SIZE];
    getAbs(input, abscoeff);

    size_t* p = new size_t[DATA_SIZE];

    gsl_sort_index(p, abscoeff, 1, DATA_SIZE);

    for (int indx_sig = 0; (indx_sig + NC) < DATA_SIZE; indx_sig++)
    {
        input[p[indx_sig]] = 0;
    }

    // print
    cout << "After removing: " << endl;
    print(input);

    gsl_wavelet_transform_inverse(w, input, 1, DATA_SIZE, work);

    cout << "Processed: " << endl;
    print(input);

    if(input)
    {
        delete[] input;
        input = nullptr;
    }
    if(abscoeff)
    {
        delete[] abscoeff;
        abscoeff = nullptr;
    } 
    if(p)
    {
        delete[] p;
        p = nullptr;
    }

    gsl_wavelet_free(w);
    gsl_wavelet_workspace_free(work);

    return 0;
}//main
