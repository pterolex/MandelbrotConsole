#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>

using namespace std;

int drawFractal(double positiveImaginary,double negativeImaginary,double positiveReal, double negativeReal,bool drawToConsole)
{
    double realCoord, imagCoord;
    double realTemp, imagTemp, realTemp2, arg;
    double imaginaryStep = 0.05;
    double realStep = 0.03;
    int iterations,columns=0,lines=0;
    columns = ceil((abs(positiveReal)+abs(negativeReal))/realStep);
    lines = ceil((abs(positiveImaginary)+abs(negativeImaginary))/imaginaryStep);
    int imageSize = columns*lines;
    int operations;
    char *image = new char[imageSize];
    if (drawToConsole)
    {
        cout << "Calculated columns: "<<columns<<"\n";
        cout << "Calculated lines: "<<lines<<"\n"; 
    }
    imagCoord=positiveImaginary;
    for (int i=0;i<lines;i++)
    {
        realCoord = positiveReal;
        for (int j=0; j<columns;j++ )
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
            if (drawToConsole)
            {
                switch (iterations % 4)
                {
                    case 0:
                        //cout<<"\33[0;31m"<<".";
                        image[i*columns+j]='.';
                    break;
                    case 1:
                        //cout<<"\33[0;32m"<<"o";
                        image[i*columns+j]='.';
                    break;
                    case 2:
                        //cout<<"\33[0;33m"<<"0";
                        image[i*columns+j]='0';
                    break;
                    case 3:
                        //cout<<"\33[0;35m"<<"@";
                        image[i*columns+j]='@';
                    break;
                }
            }
            realCoord = positiveReal -(j+1)*realStep;
        }
        imagCoord = positiveImaginary - (i+1)*imaginaryStep;
    }
   if (drawToConsole)
    {
        for (int i=0;i<lines;i++)
        {
            for (int j = 0; j < columns; j++)
                cout<<image[i*columns+j];
            cout<<"\n";
        }
    }
    return lines*columns; 
}

int main()
{ 
    int userChoice;
    cout << "@@@ The program draws the Mandelbrot set in console\n";
    cout << "Choose an option:\n 1. Draw Mandelbrot set\n 2. Benchmark and write results to the output file\n 3. Exit\n";
    cin >> userChoice;
    switch (userChoice)
    {
        case 1:
        {
            drawFractal(2,-2.2,2,-2,true);
        }
        break;
        case 2:
        {
            clock_t t;
            double positiveImaginary, negativeImaginary, positiveReal, negativeReal;
            ifstream dataFile("input.txt");
            ofstream outputFile("output.txt");
            while (!dataFile.eof())
            {
                dataFile >> positiveImaginary >> negativeImaginary>>positiveReal>>negativeReal;
                t = clock();
                int symbols = drawFractal(positiveImaginary,negativeImaginary,positiveReal,negativeReal,false);
                t = clock() - t;
                cout <<(((float)t)/CLOCKS_PER_SEC)<<"\n";
                outputFile << "Params: "<<  positiveImaginary <<" "<< negativeImaginary<<" "<<positiveReal<<" "<<negativeReal<<" Symbols: "<<symbols;
                outputFile << " Time: "<<(((float)t)/CLOCKS_PER_SEC)<<"\n";
            }
        }
        break;
        default:
            cout << "Your choice is wrong!";
    }
    return 0;
}