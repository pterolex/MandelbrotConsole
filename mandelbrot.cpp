#include <iostream>
#include <time.h>

using namespace std;

int main ()
{
	time_t start,end;
	time (&start);
    double realCoord, imagCoord;
    double realTemp, imagTemp, realTemp2, arg;
    int iterations;
    for (imagCoord = 3; imagCoord >= -3; imagCoord -= 0.05)
    {
        for (realCoord = -1; realCoord <= 1.77; realCoord += 0.03)
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
                    cout<<"\33[0;34m"<<"@";
                break;
                    }
        }
        cout<<"\n";
    }
    time (&end);
	double dif = difftime (end,start);
	printf ("Elasped time is %.2lf seconds.", dif );
}