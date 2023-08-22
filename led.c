#include <reg51.h>

void ms_delay(unsigned int time);

void main()
{

    while (1)
    {
        P1 = 0x0ff;
        ms_delay(1000);
        P1 = 0x00; 
        ms_delay(1000);
    }
}

void ms_delay(unsigned int time)
{
    unsigned int i, j;
    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 113; j++);
    }
}