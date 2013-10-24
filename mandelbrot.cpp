#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <string.h>

using namespace std;

void drawFractal(double positiveImaginary,double negativeImaginary,double positiveReal, double negativeReal)
{
    double realCoord, imagCoord;
    double realTemp, imagTemp, realTemp2, arg;
    double imaginaryStep = 0.05;
    double realStep = 0.03;
    int iterations;
    for (imagCoord = positiveImaginary; imagCoord >= negativeImaginary; imagCoord -= imaginaryStep)
    {
        for (realCoord = positiveReal; realCoord >= negativeReal; realCoord -= realStep)
        {
            iterations = 0;
            realTemp = realCoord;
            imagTemp = imagCoord;
            arg = (realCoord * realCoord) + (imagCoord * imagCoord);
            while ((arg < 4) && (iterations < 40))
            {
                realTemp2 = (realTemp * realTemp) - (imagTemp * imagTemp) - realCoord;
                imagTemp = (2 * realTemp * imagTemp) - imagCoord;
                realTemp = realTemp2;
                arg = (realTemp * realTemp) + (imagTemp * imagTemp);
                iterations += 1;
            }
            switch (iterations % 4)
            {
                case 0:
                    cout<<"\33[0;31m"<<".";
                break;
                case 1:
                    cout<<"\33[0;32m"<<"o";
                break;
                case 2:
                    cout<<"\33[0;33m"<<"0";
                break;
                case 3:
                    cout<<"\33[0;35m"<<"@";
                break;
                    }
        }
        cout<<"\n";
    }
}

void 
int main()
{
    time_t start,end;
    int times = 1000;
    double positiveImaginary, negativeImaginary, positiveReal, negativeReal;
    ifstream dataFile("input.txt");
    ofstream outputFile("output.txt");
    while (!dataFile.eof())
    {
        dataFile >> positiveImaginary >> negativeImaginary>>positiveReal>>negativeReal;
        time (&start);
        for (int i=0;i<=times;i++)
        {
            drawFractal(positiveImaginary,negativeImaginary,positiveReal,negativeReal);
        }
        time (&end);
        double dif = difftime (end,start);
        outputFile << dif <<"\n";
    }

}