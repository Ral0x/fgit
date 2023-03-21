#include <iostream>
#include <stdio.h>
#include <math.h>
#include <mpreal.h>
using namespace std;
using namespace mpfr;
int main()
{
    mpfr_set_default_prec(800);
    mpreal x =591286729879;
    mpreal y =956722026041;
    mpreal res = -1/(pow(y,6)-3*x*pow(y,5)+5*pow(x,3)*pow(y,3)-3*pow(x,5)*y-pow(x,6))-1;
    cout<<"Result with MPFR : " << res << endl;
}
//./Program
//g++ -Wall -o Program MPFRLab.cpp -lmpfr -lgmp
