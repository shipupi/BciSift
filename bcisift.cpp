#include <iostream>
#include <fstream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include <QGlobal.h>
#include <QTime>

int randInt(int low, int high)
    {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}


void salt(cv::Mat &image, int n)
{
    for(int k=0; k<n; k++)
    {
        int i = rand() % image.cols;
        int j = rand() % image.rows;

        if (image.channels() == 1) {
            // grey image
            image.at<uchar>(j,i) = 255;

        } else if (image.channels() == 3)
        {
            // (row, column)
            image.at<cv::Vec3b>(j,i)[0] = 255;
            image.at<cv::Vec3b>(j,i)[1] = 255;
            image.at<cv::Vec3b>(j,i)[2] = 255;
        }
    }
}

cv::Mat subsample(cv::Mat image)
{
    //cv::Mat featureImage(240,imagewidth,CV_8U,cv::Scalar(0));

    cv::Mat featureImage;

    cv::pyrDown( image, featureImage, cv::Size( image.cols/2, image.rows/2 ));

    //cv::imshow("Subsampled", featureImage);

    return featureImage;
}


cv::Mat scalespace(std::string name, cv::Mat image, const int imageheight, const int imagewidth, float k)
{
    cv::Mat featureImage(imageheight,imagewidth,CV_8U,cv::Scalar(0));

    float sigmao = 1.6f;
    float sigma = sigmao * k;
    float size = 8 * sigma + 1;

    std::cout << "Sigma:" << sigma << " Size:" << size << std::endl;

    // Kernel must be odd
    if ( ((int)size)% 2 == 0) size+=1;

    GaussianBlur( image, featureImage, cv::Size( size, size ), sigma, sigma );

    cv::imshow(name, featureImage);

    return featureImage;
}

void setValAt(cv::Mat mask, int y, int x, int value)
{
    cv::Size s = mask.size();

    if ((0 < y && y < s.height) &&
        (0 < x && x < s.width))
    {
        mask.at<uchar>(y,x)=value;
    }
}



cv::Mat patchfromthis(cv::Mat mask, int i, int j, int value)
{
    setValAt(mask,i-1,j-1,value);
    setValAt(mask,i-1,j,value);
    setValAt(mask,i-1,j+1,value);
    setValAt(mask,i,j-1,value);
    setValAt(mask,i,j,value);
    setValAt(mask,i,j+1,value);
    setValAt(mask,i+1,j-1,value);
    setValAt(mask,i+1,j,value);
    setValAt(mask,i+1,j+1,value);

    return mask;
}

cv::Mat patchfromthis(cv::Mat mask, int i, int j)
{
    return patchfromthis(mask,i,j,1);
}

cv::Mat patch(cv::Mat tmp, int i, int j, int value)
{
    cv::Size s = tmp.size();

    cv::Mat mask(s.height,s.width,CV_8U,cv::Scalar(0));

    return patchfromthis(mask,i,j,value);
}

cv::Mat patch(cv::Mat tmp, int i, int j)
{
    return patch(tmp,i,j,1);
}

void extremascale(cv::Mat tmp, cv::Point minLoc, cv::Point maxLoc, double &minVal, double &maxVal )
{
    cv::Point minLoc2, maxLoc2;

    // We need that the patch includes both minLoc and maxLoc
    cv::Mat mask = patch(tmp, minLoc.y, minLoc.x);
    mask = patchfromthis(mask,maxLoc.y, maxLoc.x);

    cv::minMaxLoc(tmp, &minVal, &maxVal,&minLoc2 , &maxLoc2, mask);
}

bool extremascale(cv::Mat tmp, cv::Point location, double maxVal)
{
    cv::Size s = tmp.size();

    cv::Mat mask(s.height,s.width,CV_8U,cv::Scalar(0));

    double minVal; cv::Point minLoc, location2;
    double maxVal2;

    //cv::minMaxLoc(tmp, &minVal, &maxVal2,&minLoc , &location2);
    //location = location2;

    setValAt(mask,location.y-1,location.x-1,1);
    setValAt(mask,location.y-1,location.x,1);
    setValAt(mask,location.y-1,location.x+1,1);
    setValAt(mask,location.y,location.x-1,1);
    setValAt(mask,location.y,location.x,1);
    setValAt(mask,location.y,location.x+1,1);
    setValAt(mask,location.y+1,location.x-1,1);
    setValAt(mask,location.y+1,location.x,1);
    setValAt(mask,location.y+1,location.x+1,1);


    cv::minMaxLoc(tmp, &minVal, &maxVal2,&minLoc , &location2, mask);

    std::cout << maxVal << " >> MaxVal at Pyramidal Location:" << maxVal2 << std::endl ;

    return (maxVal>=maxVal2);
}

void showextrema(cv::Mat tmp, int scale)
{
    cv::Point location, minLoc;
    double maxVal, minVal;

    cv::minMaxLoc(tmp, &minVal, &maxVal,&minLoc , &location);

    std::cout << "MaxValue at scale " << scale << ":" << maxVal << std::endl ;
}

cv::Point addextrema(int octave,cv::Mat tmp, cv::Point location, double val)
{
    cv::Point extrema(0,0);

    tmp.at<uchar>(location.y,location.x) = 255;
    extrema.x = ((int)((float)location.x)*pow(2,octave-1));
    extrema.y = ((int)((float)location.y)*pow(2,octave-1));

    extrema.x = location.x;
    extrema.y = location.y;

    if (octave == 2) {extrema.x = extrema.x*2; extrema.y=extrema.y*2;}
    if (octave == 3) {extrema.x = extrema.x*4; extrema.y=extrema.y*4;}

    std::cout << octave <<  "- Max Location:" << val << " " << extrema << std::endl;

    return extrema;
}

void locatelocalextrema(int octave, cv::Mat tmp,cv::Mat up, cv::Mat down,std::vector<cv::Point> &extrema)
{
    cv::Size s = tmp.size();

    for(int i=1;i<s.height;i++)
        for (int j=1;j<s.width;j++)
        {
            if (i>=s.height || tmp.at<uchar>(i,j)==0)
                continue;

            cv::Point location, minLoc;
            double maxVal, minVal;

            cv::minMaxLoc(tmp, &minVal, &maxVal,&minLoc , &location, patch(tmp,i,j));

            double maxValUp, maxValDown;
            double minValUp, minValDown;

            extremascale(up,minLoc,location,minValUp, maxValUp);
            extremascale(down,minLoc,location,minValDown, maxValDown);

            if (minVal == 0 || maxVal == 0)
                continue;

            //if (minVal < minValDown && minVal < minValUp)
           // {
           //     extrema.push_back(addextrema(octave,tmp,minLoc,minVal));
           // }

            if (maxVal > maxValDown && maxVal > maxValUp)
            {
                std::cout << maxVal << " >> MaxVal at Pyramidal Location:" << maxValDown << "-" << maxValUp << std::endl ;
                extrema.push_back(addextrema(octave,tmp,location,maxVal));
            }
        }
}


void locateextrema(int octave, int howmany, std::string name,cv::Mat tmp, cv::Mat up, cv::Mat down, cv::Mat mask, std::vector<cv::Point> &extrema)
{
    cv::Point minLoc,maxLoc;
    double maxVal, minVal;

    cv::minMaxLoc(tmp, &minVal, &maxVal,&minLoc , &maxLoc, mask);

    double minValUp, minValDown;
    double maxValUp, maxValDown;

    // Lets check what happen with the patch upscale and downscale.
    //extremascale(up,   minLoc, maxLoc, minValUp,   maxValUp);
    //extremascale(down, minLoc, maxLoc, minValDown, maxValDown);



    //std::cout << minVal << "," << maxVal <<  " at " << minLoc << "," << maxLoc << " vs " << "[" << minValUp << "," << minValDown << "] and [" << maxValUp << "," << maxValDown << "]" << std::endl;

    //if (minVal < minValUp && minVal < minValDown)
    {
        //extrema.push_back(addextrema(octave,tmp,minLoc,minVal));
    }

    //if (maxVal > maxValUp && maxVal > maxValDown)
    {
        extrema.push_back(addextrema(octave,tmp,maxLoc,maxVal));
    }

    if (howmany == 6)
    {
        setValAt(mask,maxLoc.y,maxLoc.x,0);
        locateextrema(octave,--howmany,name,tmp,up,down,mask,extrema);
        threshold( tmp, tmp, 5, 100,0 );
        imshow(name, tmp);
    } else if (howmany > 0)
    {
        setValAt(mask,maxLoc.y,maxLoc.x,0);
        locateextrema(octave,--howmany,name,tmp,up,down,mask,extrema);
    }

    //imshow(name, tmp);

    //cv::waitKey(40000);
}


cv::Mat heatup(cv::Mat image, float threshold=20, float heat=150)
{
    cv::Size s = image.size();

    for(int y=0;y<s.height;y++)
        for (int x=0;x<s.width;x++)
        {
            if (image.at<uchar>(y,x) > threshold)
               image.at<uchar>(y,x) += heat;
        }

    return image;
}

cv::Mat findkeypoint(int octave, cv::Mat image, const int imageheight, const int imagewidth, std::vector<cv::Point> &extrema)
{
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> DoG;

    std::string wtitle ( octave,'-');

    // Primera octava, son 5 escalas.
    images.push_back(scalespace(wtitle + "1",image,imageheight, imagewidth, pow(2,octave-1)*pow(sqrt(2),1)/(1.6*2)));
    images.push_back(scalespace(wtitle + "2",image,imageheight, imagewidth, pow(2,octave-1)*pow(sqrt(2),2)/(1.6*2)));
    images.push_back(scalespace(wtitle + "3",image,imageheight, imagewidth, pow(2,octave-1)*pow(sqrt(2),3)/(1.6*2)));
    images.push_back(scalespace(wtitle + "4",image,imageheight, imagewidth, pow(2,octave-1)*pow(sqrt(2),4)/(1.6*2)));
    images.push_back(scalespace(wtitle + "5",image,imageheight, imagewidth, pow(2,octave-1)*pow(sqrt(2),5)/(1.6*2)));

    // As per the Lowe's paper, it is the third image from top the one who you will use to create the same.
    cv::Mat Vnspo = images[4];

    DoG.push_back(images[1]-images[0]);
    DoG.push_back(images[2]-images[1]);
    DoG.push_back(images[3]-images[2]);
    DoG.push_back(images[4]-images[3]);

    cv::Mat tmp = DoG[1];

    showextrema(DoG[0],1);
    showextrema(DoG[1],2);
    showextrema(DoG[2],3);
    showextrema(DoG[3],4);

    //extrema.push_back(locateextrema(octave,wtitle + "at 2",tmp,DoG[0],DoG[2]));
    //extrema.push_back(locateextrema(octave,wtitle + "at 3",DoG[2],DoG[1],DoG[3]));

    cv::Mat mask(imageheight,imagewidth,CV_8U,cv::Scalar(1));

    locateextrema(octave,6,wtitle + "at 2",DoG[1],DoG[0],DoG[2], mask, extrema);
    locateextrema(octave,6,wtitle + "at 3",DoG[2],DoG[1],DoG[3], mask, extrema);

    if (extrema.size() == 0)
        exit(1);

    //locatelocalextrema(octave,tmp,DoG[0],DoG[2],extrema);
    //locatelocalextrema(octave,DoG[2],DoG[1],DoG[3],extrema);

    Vnspo = heatup(Vnspo);

    return subsample(Vnspo);
}

const int thresh = 20;

struct result
{
    int size;
    int hits;
};

result handmadesift(cv::Mat image, const int imageheight, const int imagewidth, const int numOctaves, std::vector<int> event)
{
    cv::Mat Vnspo=image;

    std::vector<cv::Point> extrema;
    cv::Scalar color(255,255,255);

    int hits=0;


    for(int l=1;l<numOctaves+1;l++)
    {
        Vnspo = findkeypoint(l, Vnspo, imageheight/pow(2,l-1), imagewidth/pow(2,l-1), extrema);

        for(unsigned int i=0;i<extrema.size();i++)
        {
            if (extrema[i].x > 0 && extrema[i].y > 0)
            {
                std::cout << "Extrema[" << i << "]:" << extrema[i] << std::endl;
                double radi=10;
                cv::circle(image,extrema[i],radi,color);

                for(int k=0;k<event.size();k++)
                {
                    if ( ((event[k]-thresh)<extrema[i].y) && (extrema[i].y<event[k]+thresh) )
                    {
                        hits++;
                    }
                }
            }
        }
    }

    std::cout << "Size:" << extrema.size() << std::endl;

    cv::imshow("handMadeSift", image);

    cv::imwrite("handmadesift.png", image);

    result r;

    r.hits = hits;
    r.size = extrema.size();

    return r;
}

result featuredetector(cv::Mat image, const int imageheight, const int imagewidth, std::vector<int> event)
{
    // This is the full image
    cv::Mat featureImage(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    std::vector<cv::KeyPoint> keypoints;

    cv::FastFeatureDetector fast(40);
    fast.detect(image,keypoints);

    cv::drawKeypoints( image,
                       keypoints,
                       featureImage,
                       cv::Scalar(255,255,255),
                       cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    int hits=0;
    for(int p=0;p<event.size();p++)
    {
        for(int k=0;k<keypoints.size();k++)
        {
            if ( ((event[p]-thresh)<keypoints[k].pt.y) && (keypoints[k].pt.y<event[p]+thresh) )
            {
                hits++;
            }
        }
    }

    cv::imshow("FAST", featureImage);
    cv::imwrite("fast.png", featureImage);

    result r;

    r.hits = hits;
    r.size = keypoints.size();

    return r;
}

result surf(cv::Mat image, const int imageheight, const int imagewidth, std::vector<int> event)
{
    // This is the full image
    cv::Mat featureImage(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    std::vector<cv::KeyPoint> keypoints;


    cv::SurfFeatureDetector surf(2500.);
    surf.detect(image,keypoints);

    cv::drawKeypoints( image,
                       keypoints,
                       featureImage,
                       cv::Scalar(255,255,255),
                       cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    int hits=0;
    for(int p=0;p<event.size();p++)
    {
        for(int k=0;k<keypoints.size();k++)
        {
            if ( ((event[p]-thresh)<keypoints[k].pt.y) && (keypoints[k].pt.y<event[p]+thresh) )
            {
                hits++;
            }
        }
    }


    cv::imshow("SURF", featureImage);
    cv::imwrite("surf.png",featureImage);

    result r;

    r.hits = hits;
    r.size = keypoints.size();

    return r;
}

result sift(cv::Mat image, const int imageheight, const int imagewidth, int edgeresponse, std::vector<int> event)
{
    // This is the full image
    cv::Mat featureImage(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    std::vector<cv::KeyPoint> keypoints;

    cv::SiftFeatureDetector sift(0.03,edgeresponse);
    sift.detect(image,keypoints);

    cv::drawKeypoints( image,
                       keypoints,
                       featureImage,
                       cv::Scalar(255,255,255),
                       cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    cv::imshow("SIFT", featureImage);
    cv::imwrite("sift.png",featureImage);

    int hits=0;
    for(int p=0;p<event.size();p++)
    {
        std::cout << "Event at:" << event[p] << std::endl;
        for(int k=0;k<keypoints.size();k++)
        {
            std::cout <<  "Event:" << event[p]-thresh << std::endl;
            std::cout  << "Keypoint:" <<  keypoints[k].pt.x << std::endl;
            std::cout <<  "Event:" << event[p]+thresh << std::endl;
            if ( ((event[p]-thresh)<keypoints[k].pt.x) && (keypoints[k].pt.x<(event[p]+thresh)) )
            {
                hits++;
                std::cout << "Hit!" << std::endl;
            }
        }
    }

    std::cout << "Hits:" << hits;

    //if (hits>0)
    //{
    //    cv::waitKey(20000);
    //    exit(1);
    //}

    result r;

    r.hits = hits;
    r.size = keypoints.size();

    return r;
}

int bcisift(int option)
{
    const int imagewidth = 320;
    const int imageheight= 240;

    const int numOctaves = 2;

    const int timestep = 10;

    std::vector<int> event;

    int err=0;
    int autotest=0;

    char logfilename[256];

    int edgeresponse=10;

    // 1 La imagen queda igual
    // 2 La imagen se ajusta a toda la pantalla y se resizea.
    cv::namedWindow("BrainWaves",2);

    cv::Point pt1(1,100);
    cv::Point pt2(10,100);

    int idx = 2;

    cv::Mat image(imageheight,imagewidth,CV_8U,cv::Scalar(0));
    cv::Scalar color(255,255,255);

    static int delta=0;
    static double psd=0;
    static int times=0;

    for(;;)
    {

        cv::line(image, pt1, pt2,color);

        int key = cv::waitKey(30);

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
                return 1;
                break;
        }

        if (!(0 < err && err<=50))
            err=0;

        if (!(0 < edgeresponse && edgeresponse<=90))
            edgeresponse=0;

        // Must have the same window name.
        cv::imshow("BrainWaves", image);


        pt1=pt2;

        if (delta>0 || randInt(0,10) == 5)
        {
            int p300[] = {0,0,-5,10,30,25,10,5,0,0,0};

            psd+=p300[delta];

            cv::Point pt3(idx+=timestep,100+p300[delta++]);
            pt2=pt3;

            if (delta==5) event.push_back(idx);
            if (delta>=11) delta=0; //Reset

            times++;
        } else
        {

            cv::Point pt3(idx+=timestep,100+randInt(50-err,50+err)-50);
            pt2=pt3;
        }

        if (idx>=imagewidth)
        {
            result r;

            cv::imwrite("base.png", image);


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
            }

            std::cout << "-------------" << std::endl;

            cv::Mat image2(imageheight,imagewidth,CV_8U,cv::Scalar(0));
            image = image2;
            idx=3;
            cv::Point pt4(1,100);
            cv::Point pt5(1+timestep,100);

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

        // Use me to log the time "t" (y position)
        //std::cout << idx << std::endl;



    }

    //cv::imwrite("output.bmp", image);

    return 1;
}

int showLine(std::string imagename)
{
   cv::namedWindow("BrainWaves",1);

   cv::Point pt1(100,100);
   cv::Point pt2(150,100);
   cv::Mat image(240,320,CV_8U,cv::Scalar(0));
   cv::Scalar color(255,255,255);

   cv::line(image, pt1, pt2,color);


   if(! image.data )                              // Check for invalid input
   {
       std::cout <<  "Could not open or find the image " << imagename << std::endl ;
       return -1;
   }


   std::cout << "size:" << image.size().height << " , " << image.size().width << std::endl;


   cv::imshow("My IMage", image);



   cv::waitKey(5000);

   return 1;

}

int mafin(int argc, char *argv[])
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
        if (_strcmpi(argv[1],"sift")==0)
            option = 1;
        else if (_strcmpi(argv[1],"surf")==0)
            option = 2;
        else if (_strcmpi(argv[1],"fast")==0)
            option = 3;
        else if (_strcmpi(argv[1],"handmade")==0)
            option = 4;
    }
    else
    {
        std::cout << "bcisift {sift|surf|fast|handmade}" << std::endl;
        return -1;
    }

    return bcisift(option);

}
