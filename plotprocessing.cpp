#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/features2d/features2d.hpp>

#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <opencv2/ml.hpp>


#include <QGlobal.h>
#include <QTime>

using namespace cv::ml;


std::vector<int> readlabels(int subject)
{
    char filename[256];
    sprintf(filename, "labelrange%d.csv", subject);

    std::ifstream inputfile(filename);
    std::string current_line;
    // vector allows you to add data without knowing the exact size beforehand
    std::vector<int>  all_data;
    // Start reading lines as long as there are lines in the file
    while(std::getline(inputfile, current_line)){
       // Now inside each line we need to seperate the cols
       std::stringstream temp(current_line);
       std::string single_value;
       while(std::getline(temp,single_value,',')){
            // convert the string element to a integer value
            all_data.push_back(atoi(single_value.c_str()));
       }
    }

    return all_data;
}

/**
void readtable()
{
    std::ifstream inputfile("labels.csv");
    std::string current_line;
    // vector allows you to add data without knowing the exact size beforehand
    std::vector< vector<int> > all_data;
    // Start reading lines as long as there are lines in the file
    while(std::getline(inputfile, current_line)){
       // Now inside each line we need to seperate the cols
       std::vector<int> values;
       std::stringstream temp(current_line);
       std::string single_value;
       while(std::getline(temp,single_value,',')){
            // convert the string element to a integer value
            values.push_back(atoi(single_value.c_str()));
       }
       // add the row to the complete data vector
       all_data.push_back(values);
    }

    // Now add all the data into a Mat element
    Mat vect = Mat::zeros((int)all_data.size(), (int)all_data[0].size(), CV_8UC1);
    // Loop over vectors and add the data
    for(int rows = 0; row < (int)all_data.size(); rows++){
       for(int cols= 0; cols< (int)all_data[0].size(); cols++){
          vect.at<uchar>(rows,cols) = all_data[rows][cols];
       }
    }
}**/



cv::Mat processplot(char *filename)
{
    cv::Mat src;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

    //infile = std::ifstream(filename);
    src = cv::imread( filename );

    if( !src.data )
    {
        printf("Error!\n");
        return descriptors; }

    cv::Ptr<cv::xfeatures2d::SIFT> detector = cv::xfeatures2d::SIFT::create(0,70,0.04,2,1.6);

    detector->detect( src, keypoints);

    detector->compute(src,keypoints,descriptors);

    return descriptors;
}

void process()
{

    char filename[256];

    for(int s=1;s<=1;s++)
    {
        std::vector<cv::Mat> M;
        cv::Mat trainingVector;
        cv::Mat labelMat;
        std::vector<int> labels = readlabels(s);
        for(int e=1;e<=180;e++)
        {
            for(int c=1;c<=1;c++)
            {
                int l=labels[e-1];
                /// Load an image
                sprintf(filename,"/Users/rramele/Data/Plots/s.%d.e.%d.l.%d.c.%d.tif",s,e,l,c);
                printf("Processing %d.%d.%d.%d:%s\n",s,e,l,c,filename);
                cv::Mat descriptors = processplot(filename);
                trainingVector.push_back(descriptors);
                for(int i=0;i<descriptors.size().height;i++)
                {
                    labelMat.push_back(l);
                }
                M.push_back(descriptors);

            }
        }

        cv::Mat testVector;
        cv::Mat expectedLabelMat;
        for(int e=181;e<=420;e++)
        {
            for(int c=1;c<=1;c++)
            {
                int l=labels[e-1];
                /// Load an image
                sprintf(filename,"/Users/rramele/Data/Plots/s.%d.e.%d.l.%d.c.%d.tif",s,e,l,c);
                printf("Processing %d.%d.%d.%d:%s\n",s,e,l,c,filename);
                cv::Mat descriptors = processplot(filename);
                testVector.push_back(descriptors);
                for(int i=0;i<descriptors.size().height;i++)
                {
                    expectedLabelMat.push_back(l);
                }
                M.push_back(descriptors);
            }
        }

        cv::Ptr<SVM> svm = SVM::create();
        svm->setType(SVM::C_SVC);
        svm->setKernel(SVM::LINEAR);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));


        printf("Size: %d, %d\n", trainingVector.size().width, trainingVector.size().height);
        printf("Size: %d, %d\n", labelMat.size().width, labelMat.size().height);

        svm->train(trainingVector, ROW_SAMPLE, labelMat);


        int accuracies=0;

        for(int i=0;i<240;i++)
        {
            float response = svm->predict(testVector.row(i));
            printf("Response %10.6f\n", response);
            int expected = expectedLabelMat.at<int>(1,1);

            if (response==expected) accuracies++;

        }


        printf ("Success %10.6f\n", (float)accuracies/240);






   /**     1 # create BFMatcher object
        2 bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
        3
        4 # Match descriptors.
        5 matches = bf.match(des1,des2)
        6
        7 # Sort them in the order of their distance.
        8 matches = sorted(matches, key = lambda x:x.distance)
        9
       10 # Draw first 10 matches.
       11 img3 = cv2.drawMatches(img1,kp1,img2,kp2,matches[:10], flags=2)
       **/

    }





}
