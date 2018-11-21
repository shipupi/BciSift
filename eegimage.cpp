#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/features2d/features2d.hpp>

#include <QGlobal.h>
#include <QTime>

#include "vl/sift.h"
#include "vl/mathop.h"

#include "bcisift.cpp"

const int imagewidth = 256;
const int imageheight= 240;

const int numOctaves = 2;

// How many pixels should we skip to draw a new point.
const int timestep = 1;

std::vector<int> event;

int err=0;
int autotest=0;

char logfilename[256];

int edgeresponse=10;

static int delta=0;
static double psd=0;
static int times=0;

static double AVG=1612.0;

void
transpose_descriptor (vl_sift_pix* dst, vl_sift_pix* src)
{
  int const BO = 8 ;  /* number of orientation bins */
  int const BP = 4 ;  /* number of spatial bins     */
  int i, j, t ;

  for (j = 0 ; j < BP ; ++j) {
    int jp = BP - 1 - j ;
    for (i = 0 ; i < BP ; ++i) {
      int o  = BO * i + BP*BO * j  ;
      int op = BO * i + BP*BO * jp ;
      dst [op] = src[o] ;
      for (t = 1 ; t < BO ; ++t) {
        dst [BO - t + op] = src [t + o] ;
      }
    }
  }
}

void cvWaitKeyWrapper()
{
    int key = cv::waitKey(1);
    switch (key)
    {
        case 13:
            while ( cv::waitKey(1000) != 13);
            break;
        case '-':
            err-=1;
            break;
        case '+':
            err+=1;
            break;
        case 'w':
            edgeresponse+=1;
            break;
        case 's':
            edgeresponse-=1;
            break;
        case 't':
            autotest=1;
            break;
        case 27:
        case 'q':
            exit(-1);
            break;
    }
}

void calculate_descriptors(float *descr,cv::Mat image, int width, int height,double Sx, double Sy, bool floatDescriptors )
{
    VlSiftFilt        *filt ;
    std::vector<float> img;

    for (int i = 0; i < image.rows; ++i)
      for (int j = 0; j < image.cols; ++j)
        img.push_back(image.at<unsigned char>(i,j));

    //setverbose(10);

    filt = vl_sift_new (width, height, 1, 3, 0) ; // M is width
    //vl_dsift_process(filter, &img[0]);

    int err   = vl_sift_process_first_octave (filt, (const vl_sift_pix *)&img[0]) ;
    if (err) return ;


    vl_sift_pix  buf [128] ;
    vl_sift_pix rbuf [128] ;
    double                angles [4] ;
    int                   nangles ;
    VlSiftKeypoint        ik ;
    VlSiftKeypoint        *k ;
    double            *ikeys = 0 ;
    //bool floatDescriptors = true;
    //double descr[128];

    vl_sift_keypoint_init (filt, &ik,
                           width/2-1,
                           height/2-1,
                           1) ;
    ik.sigmax = Sx;
    ik.sigmay = Sy;

    k = &ik ;

    angles [0] = VL_PI / 2 - 0 ; // 0 is the angle
    nangles    = 1 ;

    angles[0] = 0;

    vl_sift_calc_keypoint_descriptor (filt, buf, k, angles[0]) ;
    transpose_descriptor (rbuf, buf) ;

    if (! floatDescriptors) {
        for (int j = 0 ; j < 128 ; ++j) {
          float x = 512.0F * rbuf [j] ;
          x = (x < 255.0F) ? x : 255.0F ;
          ((vl_uint8*)descr) [128 * 1 + j] = (vl_uint8) x ;
        }
    } else {
        for (int j = 0 ; j < 128 ; ++j) {
          float x = 512.0F * rbuf [j] ;
          ((float*)descr) [128 * 1 + j] = x ;
        }
    }

    vl_sift_delete (filt) ;

}


void calculate_descriptors(cv::Mat image)
{
    float descr[128];
    calculate_descriptors(descr, image, imagewidth, imageheight, 1, 1, true);
}

double mean(double signal[], int length)
{
    cv::Mat A(length,1,CV_64F,signal);
    cv::Scalar mn = cv::mean(A);

    return mn.val[0];
}



int eegimage(float descr[128],double signal[], int length, int gamma, std::string windowname)
{
    cv::namedWindow(windowname,cv::WINDOW_NORMAL);

    int height;
    int width;

    height = 240;
    width = length;

    cv::Mat image(height,width,CV_8U,cv::Scalar(0));
    cv::Scalar color(255,255,255);

    int idx = 1;

    int zerolevel=height/2;

    double avg = mean(signal,length);

    for(idx=0;idx<width-1;idx++)
    {
        //cv::Point pt3(idx+=timestep,100+randInt(50-err,50+err)-50);

        double value = zerolevel+(int)(signal[idx]*gamma) - (int)avg-1;
        double valuenext = zerolevel+(int)(signal[idx+1]*gamma) - (int)avg-1;

        if (value<0) value = 1;
        if (value>height) value = (height-1);

        if (valuenext<0) valuenext = 1;
        if (valuenext>height) valuenext = (height-1);

        //std::cout<<"Idx:" << idx << std::endl;

        cv::Point pt1(idx,value);
        cv::Point pt2(idx+1,valuenext);

        // Draw a new line between pt1 and pt2.  The line routine uses Brasenham algorithm.
        cv::line(image, pt1, pt2,color);
    }


    // The Window Name must be the same.
    cv::imshow(windowname, image);


    std::cout << "-------------" << std::endl;

    calculate_descriptors(descr,image,imagewidth,imageheight,1,1,true);
    cvWaitKeyWrapper();

    return 1;
}

int eegimage(float descr[128],double signal[],int length, int gamma,int windowlabelid)
{
    // 1 La imagen queda igual
    // 2 La imagen se ajusta a toda la pantalla y se resizea.
    char buff[100];
    snprintf(buff, sizeof(buff), "%d", windowlabelid+1);
    std::string windowname = buff;
    return eegimage(descr,signal,length,gamma, windowname);
}

int eegimage(double signal[],int length, int gamma,int label)
{
    float descr[128];
    return eegimage(descr,signal,length, gamma,label);
}


int eegimage(double avg, double data)
{
    // 1 La imagen queda igual
    // 2 La imagen se ajusta a toda la pantalla y se resizea.
    cv::namedWindow("BrainWaves",cv::WINDOW_NORMAL);

    static cv::Mat image(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    static cv::Scalar color(255,255,255);

    static int idx = 1;

    int heightvalue=imageheight/2;

    {
        //cv::Point pt3(idx+=timestep,100+randInt(50-err,50+err)-50);

        heightvalue = (imageheight/2)+(int)data - (int)avg-1;

        if (heightvalue<0) heightvalue = 1;
        if (heightvalue>imageheight) heightvalue = (imageheight-1);

        std::cout<<"Idx:" << idx << std::endl;
    }

    static cv::Point pt1(idx,heightvalue);
    static cv::Point pt2(idx,heightvalue);

    cv::Point pt3(idx,heightvalue);
    pt2=pt3;

    // Draw a new line between pt1 and pt2.  The line routine uses Brasenham algorithm.
    cv::line(image, pt1, pt2,color);

    // The Window Name must be the same.
    cv::imshow("BrainWaves", image);


    pt1=pt2;
    idx+=timestep;

    if (idx>imagewidth)
    {
        std::cout << "-------------" << std::endl;


        calculate_descriptors(image);


        cv::Mat image2(imageheight,imagewidth,CV_8U,cv::Scalar(0));
        image = image2;

        idx=1;

        cv::Point pt4(1,heightvalue);

        pt1=pt4;
        pt2=pt4;

        cvWaitKeyWrapper();

    }

    return 1;

}



int eegiamage(double avg, double data)
{
    int option = 1;

    // 1 La imagen queda igual
    // 2 La imagen se ajusta a toda la pantalla y se resizea.
    cv::namedWindow("BrainWaves",2);

    static cv::Point pt1(1,imageheight/2);
    static cv::Point pt2(1+timestep,imageheight/2);

    static int idx = 2;

    static cv::Mat image(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    static cv::Scalar color(255,255,255);


    // Draw a new line between pt1 and pt2.  The line routine uses Brasenham algorithm.
    cv::line(image, pt1, pt2,color);


    int key = cv::waitKey(3000);

    switch (key)
    {
        case 13:
            while ( cv::waitKey(1000) != 13);
            break;
        case '-':
            err-=1;
            break;
        case '+':
            err+=1;
            break;
        case 'w':
            edgeresponse+=1;
            break;
        case 's':
            edgeresponse-=1;
            break;
        case 't':
            autotest=1;
            break;
        case 27:
            exit(-1);
            break;
    }

    // Err is a standard deviation which can be adjusted.
    if (!(0 < err && err<=50))
        err=0;

    // SIFT parameters
    if (!(0 < edgeresponse && edgeresponse<=90))
        edgeresponse=0;

    // The Window Name must be the same.
    cv::imshow("BrainWaves", image);


    pt1=pt2;

    {
        //cv::Point pt3(idx+=timestep,100+randInt(50-err,50+err)-50);

        int val = (imageheight/2)+(int)data - (int)avg;

        if (val<0) val = 1;
        if (val>imageheight) val = (imageheight-1);

        std::cout<<"Idx:" << idx << std::endl;

        cv::Point pt3(idx+=timestep,val);


        pt2=pt3;
    }


    if (idx>=imagewidth)
    {
        result r;

        //cv::imwrite("base.png", image);

        /**
        switch (option)
        {
        case 1:
            r=sift(image, imageheight, imagewidth, edgeresponse, event);
            strcpy(logfilename,"sift");
            break;
        case 2:
            r=surf(image, imageheight, imagewidth, event);
            strcpy(logfilename,"surf");
            break;
        case 3:
            r=featuredetector(image, imageheight, imagewidth, event);
            strcpy(logfilename,"fast");
            break;
        default:
            r=handmadesift(image, imageheight, imagewidth, numOctaves, event);
            strcpy(logfilename,"handmade");
        }**/

        std::cout << "-------------" << std::endl;

        cv::Mat image2(imageheight,imagewidth,CV_8U,cv::Scalar(0));
        image = image2;
        idx=3;
        cv::Point pt4(1,imageheight/2);
        cv::Point pt5(1+timestep,imageheight/2);

        pt1=pt4;
        pt2=pt5;

        std::cout << event.size() << ":" << "SNR:" << (times*1.0)/(err+1) << "/" << r.hits << std::endl;

        if (autotest>0)
        {
            std::ofstream myfile;
            myfile.open (logfilename, std::ios::app | std::ios::out);
            myfile << (times*1.0)/(err+1) << " " << r.size << " " << r.hits << std::endl;
            myfile.close();
        }

        times=0;psd=0;

        event.clear();

        if (autotest>0)
        {
            autotest++;

            if (autotest % 100 == 0)
            {
                err+=1;
            }

            if (err>=49)
            {
                err=0;
                autotest=0;
                exit(1);
            }
        }
    }
}
