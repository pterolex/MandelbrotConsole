#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <mpi.h>

using namespace std;


int drawFractal(double positiveImaginary,double negativeImaginary,double positiveReal, double negativeReal,bool drawToConsole,int world_rank,int world_size)
{
    double realCoord, imagCoord;
    double realTemp, imagTemp, realTemp2, arg;
    double imaginaryStep = 0.05;
    double realStep = 0.03;
    int iterations,columns=0,lines=0,i=0,j=0;
    columns = ceil((abs(positiveReal)+abs(negativeReal))/realStep);
    lines = ceil((abs(positiveImaginary)+abs(negativeImaginary))/imaginaryStep);
    int imageSize = columns*lines;
    int operations;
    char *image = new char[imageSize];
    char *image_part = new char[imageSize];
    if (drawToConsole && world_rank == 0)
    {
        cout << "Calculated columns: "<<columns<<"\n";
        cout << "Calculated lines: "<<lines<<"\n";
        cout << "Total symbols: "<<lines*columns<<"\n";
    }  
    imagCoord=positiveImaginary;
    int startLine = world_rank*(lines/world_size);
    int endLine = (world_rank+1)*(lines/world_size);
    //cout << "Rank = "<<world_rank<<" from "<<world_size<<" # Counting from "<< startLine<<" to "<<endLine<<endl;
    for (i=startLine;i<endLine;i++)
    {
        realCoord = positiveReal;
        for (j=0; j<columns;j++ )
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
                    image[i*columns+j]='.';
                    image_part[i*columns+j]='.';
                    break;
                    case 1:
                    image[i*columns+j]='o';
                    image_part[i*columns+j]='o';
                    break;
                    case 2:
                    image[i*columns+j]='0';
                    image_part[i*columns+j]='0';
                    break;
                    case 3:
                    image[i*columns+j]='@';
                    image_part[i*columns+j]='@';
                    break;
                }
            }
            realCoord = positiveReal -(j+1)*realStep;
        }
        imagCoord = positiveImaginary - (i+1)*imaginaryStep;
    }
    if (world_rank != 0)
    {
        MPI_Send(image_part, imageSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    if (world_rank == 0)
    {
        MPI_Recv(image_part, imageSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (drawToConsole)
        {
            for (int i=0;i<ceil(lines/2)-1;i++)
            {
                for (int j = 0; j < columns; j++)
                    cout<<image[i*columns+j];
                cout<<"\n";
            }
            for (int i=ceil(lines/2);i<lines;i++)
            {
                for (int j = 0; j < columns; j++)
                    cout<<image_part[i*columns+j];
                cout<<"\n";
            }
        }
    }
    return lines*columns; 
}

int main()
{ 
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int userChoice = 1;
    if (world_rank == 0)
    {
        cout << "@@@ The program draws the Mandelbrot set in console using MPICH2\n";
        cout << "Choose an option:\n 1. Draw Mandelbrot set\n 2. Benchmark and write results to the output file\n 3. Exit\n";
        cin >> userChoice;
    }
    switch (userChoice)
    {
        case 1:
        {
            drawFractal(2,-2.2,2,-2,true,world_rank,world_size); 
        }
        break;
        case 2:
        {
            double dif;
            double positiveImaginary, negativeImaginary, positiveReal, negativeReal;
            ifstream dataFile("input.txt");
            ofstream outputFile("output_mpi.txt");
            while (!dataFile.eof())
            {
                dataFile >> positiveImaginary >> negativeImaginary>>positiveReal>>negativeReal;
                double start,end;
                int symbols = drawFractal(positiveImaginary,negativeImaginary,positiveReal,negativeReal,false,world_rank,world_size);
                dif = end - start;
                cout <<dif<<"\n";
                outputFile <<  positiveImaginary <<" "<< negativeImaginary<<" "<<positiveReal<<" "<<negativeReal<<" Symbols: "<<symbols;
                outputFile << " Time: "<<dif<<"\n";
            }
        }
        break;
        default:
        cout << "Your choice is wrong!";
    }
    MPI_Finalize();
    return 0;
}