#include <iostream>
#include <fstream>
#include <string>

#include <QGlobal.h>
#include <QTime>

#include "eegimage.h"
#include "lsl.h"
#include "plotprocessing.h"
#include "decoder.h"


int  eeglogger(char *filename, char *subject, int duration);

int randInt2(int low, int high)
    {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

int madin(int argc, char *argv[])
{
    //std::string imagename = "HappyFish.jpg";
    //cv::Mat image = cv::imread(imagename,CV_LOAD_IMAGE_COLOR);
    //cv::Mat image(240,320,CV_8U,cv::Scalar(100));

    /**
    CV_LOAD_IMAGE_UNCHANGED (<0) loads the image as is (including the alpha channel if present)
    CV_LOAD_IMAGE_GRAYSCALE ( 0) loads the image as an intensity one
    CV_LOAD_IMAGE_COLOR (>0) loads the image in the RGB format
    **/

    //salt(image, 3000);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int option=0;

    if (argc>1)
    {
        if (strcmp(argv[1],"sift")==0)
            option = 1;
        else if (strcmp(argv[1],"surf")==0)
            option = 2;
        else if (strcmp(argv[1],"fast")==0)
            option = 3;
        else if (strcmp(argv[1],"handmade")==0)
            option = 4;
    }
    else
    {
        std::cout << "bcisift {sift|surf|fast|handmade}" << std::endl;
        return -1;
    }


       for(;;)
       {
           eegimage(0,randInt2(50-3,50+3)-50);
       }

       int fileid = 1;

       std::string c;

       char filename[256];

       sprintf(filename,"C:\\Users\\rramele\\Desktop\\Data\\Rodrigo\\Baseline\\e.%d.l.1.dat", fileid);
       std::ifstream infile(filename);

       infile >> c;

       int field=0;
       while (fileid<100)
       {
           //std::cout << c << std::endl;
           //eegimage(0,randInt2(50-3,50+3)-50);

           if ((field % 22)==2)
           {
                std::cout << c << std::endl;
                eegimage(4320.0F, std::stod(c));
           }
           field++;
           if (!(infile >> c))
           {
               sprintf(filename,"C:\\Users\\rramele\\Desktop\\Data\\Rodrigo\\Baseline\\e.%d.l.1.dat", fileid++);
               infile = std::ifstream(filename);
           }
       }
       infile.close();

    return 1;
    //return eeglogger("Romulo", "Romulo", 40);

    //return bcisift(option);

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
    else if (strcmp(argv[1],"rand")==0)
    {
        //float descr[20][128];

        float *descr = new float[20*128];
        for(int i=0;i<10;i++)
        {
            double signal[256];
            memset(signal,0,sizeof(double)*256);
            eegimage(&descr[i*128],signal,256,1,1);
        }

        for(int i=10;i<20;i++)
        {
            double signal[256];
            memset(signal,0,sizeof(double)*256);
            signal[120] = signal[132] = 40;
            eegimage(&descr[i*128],signal,256,1,1);
        }

        // Joya en este punto puedo tratar de clasificarlos.
        classify(descr);

    }



    return 0;
}

