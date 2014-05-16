/*    Copyright 2013 Syed Tabish, All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include<iostream>
#include<fstream>
#include<time.h>
#include<cstdio>
#include<cstdlib>         
#include<string>
#include<cv.h>
#include<highgui.h>

#define TRAIN_DATA_SIZE 60000
#define TEST_DATA_SIZE 10000
#define DIM 28
#define LEN 28*28
#define FEATURE_LEN 67
#define KVAL 5

typedef unsigned char uchar;
typedef unsigned int uint;
typedef struct ScoreList{
  int label;
  float score;
}ScoreList;

using namespace cv;
using namespace std;

int ReadData(unsigned char**, unsigned int*,const char*,const char*);
void MouseInputHandler(int event,int x,int y,int flags,void* params);
void ClearButtonHandler(int state, void* data);
int TrainClassifier(uchar** Data, uint* Labels, float** Features);
float TestClassifier(uchar ** Data, uint* orig_lbls,float** Train_pts);
int ComputeFeature(float* FeatureVector,uchar* image);
int Predict(float* FeatureVector,float** TrainingFeatures,int knn);
ScoreList* Sort(ScoreList* array,int p, int r);
int partition(ScoreList* array, int p, int r);
int ReadInput(Mat&);
char Display(Mat&,int);
int reverse(int);

int main(int argc, char* argv[]){
  unsigned char **TrainingData = new unsigned char* [TRAIN_DATA_SIZE];
  unsigned char **TestData = new unsigned char *[TEST_DATA_SIZE];
  unsigned int *TrainingLabels = new unsigned int[TRAIN_DATA_SIZE];
  unsigned int *TestLabels = new unsigned int[TEST_DATA_SIZE];
  Mat InputPad(DIM,DIM,CV_8UC1);  
  float **Features = new float* [TRAIN_DATA_SIZE];
 
  for(int i = 0 ; i < TRAIN_DATA_SIZE;i++){
    TrainingData[i] = new unsigned char[LEN];
  } 
  
  for(int i = 0;i < TEST_DATA_SIZE;i++){
    TestData[i] = new unsigned char[LEN];
  }

  for(int i = 0; i < TRAIN_DATA_SIZE;i++){//allocate space for training 
    Features[i] = new float[FEATURE_LEN];
  } 

  const char* datafile = "train-images.idx3-ubyte";
  const char* labelfile = "train-labels.idx1-ubyte"; 

  ReadData(TrainingData,TrainingLabels,datafile,labelfile);
  Mat temp(DIM, DIM, CV_8UC1,TrainingData[0]);
  
  clock_t startTime = clock();  
  
 TrainClassifier(TrainingData,TrainingLabels,Features);

  cout <<"Time for training "<< double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
  const char* datafile_test = "t10k-images.idx3-ubyte";
  const char* labelfile_test = "t10k-labels.idx1-ubyte"; 
  ReadData(TestData, TestLabels,datafile_test,labelfile_test);

  //reading data done..............................................
  int mode = 2;
  cout<<"There are three modes:\n";
  cout<<"1)The boring and slow non-interactive mode\n";
  cout<<"2)Give input image file!\n";
  cout<<"3)The interactive mode...\n ";
  cout<<"Press any key other than 1 and 2 for the interactive mode\n";
  cin >> mode;
  switch(mode){

  case 1:{ 
      startTime = clock();
      cout <<"error rate ->"<< TestClassifier(TestData,TestLabels,Features)<<endl; 
        cout <<"Time for testing "<< double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
      break;
    }
  case 2:{
    if(argc != 2){
      cout<<"Input format is ./digtRecon <test image name>"<<endl;
      return 1;
    }
   
    Mat image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    if(!image.data){
      cout<<"error opening file!"<<endl;
      return 2;
    }
    
    Mat Inputimage(28,28,CV_8UC1);
    resize(image,Inputimage,Inputimage.size());
    //    Display(Inputimage,2);
    float * Feat = new float[FEATURE_LEN];
    uchar* Input = new uchar[LEN];
    for(int k = 0;k<28;k++){
      for(int l = 0;l < 28;l++){
	Input[k + l*28] = Inputimage.at<uchar>(l,k);
      }
    }
    ComputeFeature(Feat,Input);
    int result = Predict(Feat,Features,KVAL);
    Display(image,result);
    cout<<result<<endl;
    return 0;
    break;
  }
 
  default: {
                while(1){
		  ReadInput(InputPad); //take input from user....
		  float *Feat = new float[FEATURE_LEN];
		  uchar* Input = new uchar[LEN];
		  for(int k = 0;k < 28;k++){
		    for(int l =0;l< 28;l++){
		      Input[k + 28*l] = InputPad.at<uchar>(l,k);
		    }
		  }
		  ComputeFeature(Feat,Input);
                  int result = Predict(Feat,Features,KVAL);  
		  cout <<"you were trying to write -> "<< result<<endl;
		  char retval = Display(InputPad,result);
		  if(retval == 27) break;
		  //i know there has to be a better way but i m lazy
		  for(int k = 0;k < 28;k++){
		    for(int l =0;l< 28;l++){
		      InputPad.at<uchar>(l,k) = '\0';
		    }
		  }
		  delete[] Feat;
		  delete[] Input;
		}
    }//case default ends
  }//switch case ends
  return 0;
}

float TestClassifier(uchar ** Data, uint* orig_lbls,float** Train_pts){
  float error_rate = 0;
  int mistakes = 0, i;
  int neighbours = 5;
  int** conf = new int*[10];
  for(int i =0; i < 10;i++){
    conf[i] = new int[10];
  }
   for(int i = 0;i<10;i++){
    for(int k = 0;k < 10;k++){
      conf[i][k] = 0; 
    }
  }
  //  for(neighbours = 29;neighbours < 40;neighbours = neighbours+2){
  for(i = 0;i < TEST_DATA_SIZE; i++){
    // if((i)%4 != 0) continue;
    float *test_feature = new float[FEATURE_LEN];
    ComputeFeature(test_feature,Data[i]);
    int label = Predict(test_feature,Train_pts,neighbours);
    conf[orig_lbls[i]][label]++;
    if((uint)label != orig_lbls[i]){
      mistakes++;
      
      //}
      cout <<"out of "<<i<<"  " <<mistakes<<" mistakes commited\n";
      cout <<orig_lbls[i] << " was wrongly classified as "<< label<<endl;
      //Mat temp(28,28,CV_8UC1,Data[i]);
      //Display(temp,label);
    }
    delete[] test_feature;
  }
  error_rate = (float)mistakes/(float)i;
  cout<<"confusion matrix\n";
  for(int i = 0;i<10;i++){
    for(int k = 0;k < 10;k++){
      cout<<conf[i][k]<<" "; 
    }
    cout<<endl;
  }
  // cout<<neighbours<<" -> "<< error_rate<<endl;
  //  }
  return 4*error_rate*100;
}


int Predict(float* FeatureVector,float** TrainingFeatures,int knn){
  int training_samples = 60000;//choose how many training vectors to consider
  ScoreList* DistanceList = new ScoreList[training_samples];
  float count[10] = {0};
  for(int i = 0;i < training_samples;i++){
    float dist = 0;
    // for(int k = 0;k < 62;k++){
    for(int k = 0;k < FEATURE_LEN-1;k++){
      float temp =  (TrainingFeatures[i][k] - FeatureVector[k]);
      // dist = dist + temp*temp;
      dist = dist + abs(temp);
      //if(abs(temp) > dist) dist = abs(temp);
    }
    //dist  = sqrt(dist);
    DistanceList[i].score = dist;
    DistanceList[i].label = (int)TrainingFeatures[i][FEATURE_LEN-1];
  } 
  Sort(DistanceList,0,training_samples-1);
 /* 
//print out smallest distance values. 
for(int iter =0;iter < 15;iter++){
    cout << DistanceList[iter].score <<" ; "<<DistanceList[iter].label<<endl;
  }
 */

  for(int j = 0;j < knn;j++){
    int label = DistanceList[j].label;
    count[label] = count[label] + 1.0/pow((DistanceList[j].score),2);
  }
  int max = 0,index = 0;
  for(int j = 0;j < 10;j++){
    if(count[j] > max){
      max = count[j];
      index = j;
    }
  }
/*
//print out score of each of the digits.
  cout<<"The score of each digit is as follows...\n";
  for(int iter = 0;iter < 10;iter++){
    cout <<iter<<" -> "<< count[iter]<<endl;
  }
//  cout << index;
//*/
  delete[] DistanceList;
  return index;
}

int TrainClassifier(uchar** Data, uint* Labels, float** Features){
  for(int i = 0;i < TRAIN_DATA_SIZE;++i){
    ComputeFeature(Features[i],Data[i]);
    Features[i][FEATURE_LEN-1] = (float) Labels[i]; 
  }

 return 0;
}


int ComputeFeature(float* FeatureVector,uchar* image){
  int* numericImage = new int[28*28];
  for(int i = 0;i < LEN;i++){//convert into binary image
    if(image[i]) numericImage[i] = 1;
    else numericImage[i] = 0;
    //cout << "Just To check "<<numericImage[i]<<endl;
  }
  int Sum[4] = {0};
  for(int i = 0;i < LEN;i++){
    //compute sum in each 14x14 block.......................
   
    if(i < (LEN>>1)){//upper two quadrants;
      if((i%28)< 14) Sum[1] = Sum[1] + numericImage[i];//second quadrant
      else Sum[0] = Sum[0] + numericImage[i];//first quadrant
    }
    else{//lower two quadrants
      if((i%28) < 14) Sum[2] = Sum[2] + numericImage[i];
      else Sum[3] = Sum[3] + numericImage[i];
    }
  }
  float TotalSum = Sum[0] + Sum[1] + Sum[2] + Sum[3];
  TotalSum = TotalSum + 0.1;//prevent division by 0;
  for(int k = 0;k < 4;k++) FeatureVector[k] = (float)Sum[k]/(float)TotalSum;
    
  //computing the projections.....................
  int hor[28] = {0},ver[28] = {0};
  for(int i = 0;i < LEN;i++){
    hor[i%28] = hor[i%28] + numericImage[i];
    ver[i/28] = ver[i/28] + numericImage[i];
  }
  for(int k = 4; k < 32;k++){
    FeatureVector[k] = (float)hor[k-4]/(float)(TotalSum);
    FeatureVector[k+28] = (float)ver[k-4]/(float)(TotalSum);
  }
 
  //compute center of mass.........................
  float xm = 0,ym = 0;
  for(int i = 0; i < LEN;i++){
    xm = xm + numericImage[i]*(i%28);
    ym = ym + numericImage[i]*(i/28);
  }
  FeatureVector[60] = xm/(28*TotalSum);
  FeatureVector[61] = ym/(28*TotalSum);
  //compute spread.............
  float xvar = 0,yvar = 0;
  for(int i = 0;i < LEN; i++){
    xvar = xvar + abs(numericImage[i]*(i%28) - xm);
    yvar = yvar + abs(numericImage[i]*(i/28) - ym);
  }
  FeatureVector[62] = xvar/(28*LEN*TotalSum);
  FeatureVector[63] = yvar/(28*LEN*TotalSum);
 
  //compute symmetry...........
  int xsym = 0,ysym = 0;
  for(int i = 0;i < LEN/2;i++){
    if(numericImage[i] == numericImage[(LEN - 28*((i/28) + 1))+(i%28)]){
      /*  cout <<"i" <<i;
      cout<<" init"<<numericImage[i];
      cout<<" sym"<<numericImage[(LEN - 28*((i/28) + 1))+(i%28)];
      cout<<" ind"<<(LEN - 28*((i/28) + 1))+(i%28)<<endl;
      */
      xsym = xsym + numericImage[i];
    }
  }
  for(int i = 0;i < LEN;i++){
    if(i%28 > 14) continue;
    else if(numericImage[i%28] == numericImage[28 - (i%28)]){
      ysym = ysym + numericImage[i];
    }
  }
  FeatureVector[64] = 2*(float)xsym/(float)TotalSum;
  FeatureVector[65] = 2*(float)ysym/(float)TotalSum;
  //  cout<<"The vals " << xsym << " "<<TotalSum<<endl;
  FeatureVector[FEATURE_LEN-1] = 0;//label;
 //look at the values.....
  /*
  for(int k = 63;k < FEATURE_LEN-1;k++){
    cout<<k<<" -> "<<FeatureVector[k]<<endl;
  }
  */
  delete[] numericImage;
  return 0;
}
 


int ReadInput(Mat& InputPad){
  char KeyValue = 0;
  int DONE = 13;//ascii value of carriage return
  namedWindow("InputWindow",CV_WINDOW_NORMAL);
  setMouseCallback("InputWindow",MouseInputHandler,(void*)&InputPad);
  //createButton("Clear",ClearButtonHandler,(void*)&InputPad,CV_PUSH_BUTTON);
  while(KeyValue != DONE){
    imshow("InputWindow",InputPad);
    KeyValue = waitKey(1);
  }
  destroyWindow("InputWindow");
}

void MouseInputHandler(int event,int x,int y, int flags,void* param){
  Mat* image = (Mat*) param;
  if(x > 28 || y > 28 || x < 0 || y < 0) return;
  switch(flags){
    case(CV_EVENT_FLAG_LBUTTON):{
      
      image->at<uchar>(y,x) = 255;
    }
  }
}
/*
//unsuccessfull try
void ClearButtonHandler(int state, void* data){
  Mat* image = (Mat*) data;
  *image = Scalar(0);
}
*/

int ReadData(unsigned char** Data,unsigned int* labels,const char* datafile,const char* labelfile){
  ifstream file(datafile);
  ifstream lbls(labelfile);
  if(file.is_open()){
    int magic_number = 0;
    int image_count = 0;
    int rows = 0;
    int cols = 0;
    file.read((char*)&magic_number,sizeof(magic_number));
    magic_number = reverse(magic_number);
    file.read((char*)&image_count,sizeof(image_count));
    image_count = reverse(image_count);
    file.read((char*)&rows,sizeof(rows));
    rows = reverse(rows);
    file.read((char*)&cols,sizeof(cols));
    cols = reverse(cols);
    //cout<<magic_number<<" "<<image_count<<" "<<rows<<" "<<cols<<endl;
  
    for(int i = 0; i < image_count;i++){
      for(int t = 0;t < rows; t++){
	for(int p = 0;p < cols;p++ ){
          file.read((char*)&Data[i][t*rows + p],sizeof(uchar));//read data
	  //Threshold the data
	  if(Data[i][t*rows + p] > 40)
	    Data[i][t*rows + p] = 255;
	  else Data[i][t*rows + p] = 0;
	}
      }
      //	Mat temp(rows,cols,DataType<uchar>::type,Data[i]);
      //	Display(temp);
      /* Mat temp(rows,cols, DataType<uchar>::type,Data[i]);
      if(!(i%133)){//taking a look at some random images
        for(int j = 0;j < rows;j++){
	  for(int k = 0;k < cols;k++){
	    // cout<<(int)temp.at<uchar>(j,k)<<" ";
	    cout<<(int)Data[i][j*rows + k]<<" ";
	  }
	  cout << endl;
	}
	Display(temp);
      }
      }*/
    }
  }
  else cout<<"unable to open training data file\n";
  if(lbls.is_open()){
    int magic_number,count;
    lbls.read((char*)&magic_number,sizeof(magic_number));
    magic_number = reverse(magic_number);
    lbls.read((char*)&count,sizeof(count));
    count = reverse(count);
    for(int i = 0;i < count;i++){
      lbls.read((char*)&labels[i],sizeof(char));
    }
  }  
  else cout<<"unable to read lalbes file";
  
  file.close();
  lbls.close();
  return 0;
}

char Display(Mat& img,int lbl){
  namedWindow("Image",CV_WINDOW_NORMAL);
  char* disp = new char[16];
  sprintf(disp,"It looks Like %d",lbl);
  //disp[15] = '\0';
  imshow("Image",img);
  displayOverlay("Image",disp,0);
  char ret= waitKey();
  destroyWindow("Image");
  return ret;
}


int partition(ScoreList* array, int p, int r){
  //partitioning function for quick sort
  float pivot = array[r].score;
  int pivot_label = array[r].label;
  int it = p-1;
  for(int j = p;j < r;j++){
    if(array[j].score <= pivot){
      ++it;
      float temp_sc = array[j].score;
      int temp_lbl = array[j].label;
      array[j].score = array[it].score;
      array[j].label = array[it].label;
      array[it].score = temp_sc;
      array[it].label = temp_lbl;
    }
  }
  ++it;
  array[r].score = array[it].score;
  array[r].label = array[it].label;
  array[it].score = pivot;
  array[it].label = pivot_label;

  return it;
}

ScoreList* Sort(ScoreList* array,int p, int r){
  //quick sort routine
  if(p < r){
    int q = partition(array,p,r);
    Sort(array,p,q-1);
    Sort(array,q,r);
  }
  return array;
}

int reverse(int wrong){
  /*Conversion from big-endian to little endian system*/
  unsigned char c1, c2, c3, c4;

    c1 = wrong & 255;
    c2 = (wrong >> 8) & 255;
    c3 = (wrong >> 16) & 255;
    c4 = (wrong >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
