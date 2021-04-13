#include <iostream>
#include <fstream>
#include <string.h> 

#include "eegimage.h"
#include "lsl.h"
#include "plotprocessing.h"
#include "decoder.h"
#include "dsp.h"


int  eeglogger(char *filename, char *subject, int duration);

int randInt2(int low, int high)
    {
    // Random number between low and high
    return rand() % ((high + 1) - low) + low;
}

void randomSignal(double *signal, int window, int variance)
{
    for(int i=0;i<window;i++)
    {
        signal[i] = randInt2(50-variance,50+variance)-50;
    }
}


void testsignals()
{
    double signal[256];
    memset(signal,0,sizeof(double)*256);
    signal[120] = signal[132] = 40;
    signal[128] = -50;
    //randomSignal(signal,256,20);

    for(int i=0;i<256;i++)
    {
        std::cout << signal[i] << std::endl;
    }

    zscore(signal,256);

    for(int i=0;i<256;i++)
    {
        std::cout << signal[i] << std::endl;
    }
}





/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * export LD_LIBRARY_PATH=/Users/user/work/labstreaminglayer/build/install/lsl_Release/LSL/lib/
 * LD_LIBRARY_PATH=/Users/rramele/work/labstreaminglayer/build/install/LSL/lib
 */
int main( int argc, char **argv)
{
    //comparesignals(1,31,2,1,1,231,2,1);
    //comparehits();
    //process();
    //testdsp();

    if (argc < 2)
    {
        std::cout << "bcisift send | recv | randonline | train | online | udp | testclassify | trainclassify | testsignals | rand | eegtest" << std::endl;
        return -1;
    }


    if (strcmp(argv[1],"send")==0)
    {
        sendeegandmarkers();
    }
    else if (strcmp(argv[1],"recv")==0)
    {
        receiving();
    }
    else if (strcmp(argv[1],"randonline")==0)
    {
        for(int i=0;i<256*10;i++)
        {
            //eegimage(0,randInt2(50-6,50+6)-50);
            eegimage(0,0);
        }
    }
    else if (strcmp(argv[1],"train")==0)
    {
        trainspeller();
    }
    else if (strcmp(argv[1],"online")==0)
    {
        onlinespeller();
    }
    else if (strcmp(argv[1],"udp")==0)
    {
        udp();
    }
    else if (strcmp(argv[1],"dsp")==0)
    {
        testdsp();
    }
    else if (strcmp(argv[1],"testclassify")==0)
    {
        testclassify();
    }
    else if (strcmp(argv[1],"trainclassify")==0)
    {
        trainclassify();
    }
    else if (strcmp(argv[1],"testsignals")==0)
    {
        testsignals();
    }
    else if (strcmp(argv[1],"signal")==0)
    {
        float *descr = new float[128];

        int N =256;
        int Fs = 256;
        double signal[N];
        memset(signal,0,sizeof(double)*N);
        signal[128] = 20;
        signal[120] = signal[136] = -120;
        //randomSignal(signal,512,20);

        eegimage(&descr[0],signal,N,240,1,1,false,1);
        std::string dummy;
        std::getline(std::cin, dummy);

        printdescriptor(descr);

        for(int i=0;i<128;i++)
            printf("[%6.2f]\n", descr[i]);

    }
    else if (strcmp(argv[1],"rand")==0)
    {
        //float descr[20][128];
        int words = 7;
        int charsPerWord = 5;
        int trials = words * charsPerWord;
        int timePoints = 128;

        // Cada segmento es una fila o una columna del experimento
        int segments = 12;
        // 7 Palabras, 5 letras, 12 repeticiones / palabra, 128 divisiones de tiempo en la signal.
        float *descr = new float[trials * timePoints * segments];
        
        for(int j=0;j<trials;j++)
        {
            for(int i=0;i<6;i++)
            {
                double signal[256];
                memset(signal,0,sizeof(double)*256);
                signal[120] =  2*40;
                signal[132] =  2*40;
                signal[128] = -50*2;
                eegimage(&descr[(j*12+i)*128],signal,256,256,1,1,true,1);
            }

            for(int i=6;i<12;i++)
            {
                double signal[256];
                memset(signal,0,sizeof(double)*256);
                signal[120] = 40;
                signal[132] = 40;
                signal[128] = -50;
                //randomSignal(signal,256,20);
                eegimage(&descr[(j*12+i)*128],signal,256,256,1,1,true,1);
            }
        }

        // Joya en este punto puedo tratar de clasificarlos.
        // classify(descr, trials,3*5,4*5);

        // delete [] descr;

    }
    else if (strcmp(argv[1],"eegtest")==0)
    {
        // Brian: Aca trabajas vos
        int signalLength = 256;
        double signal[signalLength];
        memset(signal,0,sizeof(double)*signalLength);
        signal[120] =  2*40;
        signal[132] =  2*40;
        signal[128] = -50*2;
        
        // auto& image = eegimage(signal, signalLength, true, "eegtest");
        
    }



    return 0;
}

