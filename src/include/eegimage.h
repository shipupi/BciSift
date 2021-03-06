#ifndef EEGIMAGE_H
#define EEGIMAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>

int eegimage(double avg, double data);

int eegimage(double signal[], int length, int Fs, int gammat, int gamma, bool normalize, int label);

int eegimage(float *descr,double signal[], int Fs, int length, int gammat, int gamma, bool normalize, int windowlabelid);

int eegimage(float *descr,double signal[], int Fs, int length, int gammat, int gamma, bool normalize, std::string windowname);

cv::Mat eegimage(double signal[], int length, bool save,std::string name);

void printdescriptor(float *descr);

void zscore(double *signal, int length);

#endif // EEGIMAGE_H


