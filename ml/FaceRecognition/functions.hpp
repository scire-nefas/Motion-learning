#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<cstdio>
#include<queue>
#include<cmath>
#include<fstream>
#include<sys/types.h>
#include<dirent.h>
#include<cv.h>
#include<highgui.h>
#include<opencv/cvaux.h>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/core/core.hpp"
#include"opencv2/contrib/contrib.hpp"


using namespace std;
using namespace cv;

#define ROWS 32
#define COLS 32
#define DETECT_FACE 1
#define DONT_DETECT_FACE 0


int ReadImage(Mat& Image,string filename,int flag);
int DetectFace(Mat& Image, Mat& Face,int flag);
int PreProcess(Mat& Image);
int getSubDirnames(DIR* dirp, vector<string> & subdir);
int GenerateRandv(vector<bool> & randv, int  n);
int ParseData(vector<Mat> & data, vector<string> & label, vector<Mat> & testData, vector<string> & testLabelstr, string & dirname,vector<bool> & randv);
int ComputeEigenBasis(PCA& ,int , vector<Mat> & );
int ComputeCoefficients(Mat&,PCA&,Mat&);
int ExtractFeatures(Mat & trainData, Mat & trainLabel, vector<Mat> & data, vector<string> & label, PCA& pca_object);
int TestBench(Mat & finalData, Mat & finalLabel, PCA& pca, vector<Mat> & data, vector<string> & label, int vectorcount);
int SaveData(PCA & pca_object, vector<bool> & randv);
int LoadData(PCA & pca_object, vector<bool> & randv);
int KnnClassify(float & label, CvKNearest & knnMod, Mat & testImage, int & k);
int KnnTrain(CvKNearest & knnMod, Mat & trainData, Mat & trainLabel);
int Accuracy(float& accuracy, Mat& testLabel, Mat& predictedLabel);
int TestSvm(CvSVM& svm, Mat& testData, Mat& testLabel);

int TrainSvm(CvSVM& svm, Mat& trainData, Mat& trainLabel);

