/* Copyright 2013 Syed Tabish. All rights reserved.    

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
#include<cstdlib>
#include<cstdio>
#include<math.h>
#include<vector>
#include<fstream>
#include<cv.h>
#include<highgui.h>


#define DIM 3

using namespace std;
using namespace cv;

int ReadData(vector<vector<float> >&, vector<int> &,const char*, const char*);
int SingleSamplePerceptron(vector< vector<float> >&,vector<float>&,float);
int plot_points(Mat&,vector< vector<float> >&, vector<int>&);
int BatchPerceptron(vector<vector<float> >&, vector<float>&,float);
int BatchRelaxation(vector<vector<float> >&,vector<float>&,float);

int main(int argc, char* argv[]){
  const char* train_data_file = "Cat2TrainData.txt";
  const char* train_label_file = "Cat2TrainLabel.txt";
  const char* test_data_file = "Cat2TestData.txt";
  const char* test_label_file = "Cat2TestLabel.txt";

  vector< vector<float> > TrainFeatures;
  vector<int> TrainLabels;
 
  vector< vector<float> > TestFeatures;
  vector<int> TestLabels;

  vector<float> a;
  vector<float> b;

  Mat plot = Mat::zeros(150,150,CV_8UC3);

  ReadData(TrainFeatures,TrainLabels,train_data_file,train_label_file);
  ReadData(TestFeatures,TestLabels,test_data_file,test_label_file);
  // plot_points(plot,TrainFeatures,TrainLabels);
  for(int i = 0 ;i < TrainFeatures.size();i++){
    if(TrainLabels[i] == 0){
      TrainFeatures[i][0] = -1*TrainFeatures[i][0];
      TrainFeatures[i][1] = -1*TrainFeatures[i][1];
    }
  }
  
  /* for(int i = 0;i < TrainFeatures.size();i++){
    cout << "> "<<TrainFeatures[i][0]<<","<<TrainFeatures[i][1];
    cout << "--> "<<TrainLabels[i]<<endl;
    }
  */


  int choice = 3;
  cin >> choice;
  switch(choice){
    case 1:{
      SingleSamplePerceptron(TrainFeatures,a,0);
      break;
    }
    case 2:{
      BatchPerceptron(TrainFeatures,a,0);
      break;
    }
    case 3:{
      SingleSamplePerceptron(TrainFeatures,a,10);
      break;
    }
    case 4:{
      BatchPerceptron(TrainFeatures,a,10);
      break;
    }
    case 5:{
    BatchRelaxation(TrainFeatures,a,10);
    } 
  }

  cout<<"a vector"<<endl;
  for(int i = 0;i < DIM;i++){
    cout<< a[i]<<endl; 
  }
  
  return 0;
}

int BatchRelaxation(vector<vector<float> >&features,vector<float>&a,float margin){
 for(int i = 0;i < DIM;i++){
    a.push_back(rand()%7);
  }
  float eta[DIM] = {0.1,0.1,0.1};
  float* dotprod = new float[features.size()];
  int mistake_count = 0,loop_count = 0;
  bool* mistakes = new bool[features.size()];
  while(1){
    loop_count++;
    for(int i = 0;i < features.size();i++){
      //dot product computation
      dotprod[i] = 0;
      for(int j = 0;j < DIM;j++){
        dotprod[i] += a[j]*features[i][j];//calculate dot product
      } 
      //mark mistakes
      if(dotprod[i] < margin){
	mistakes[i] = true;
	mistake_count++;
      }else{
	mistakes[i] = false;
      }
    }
    // cout<< mistake_count << " mistake cout\n";
    if(mistake_count < 1) break;mistake_count = 0;
    //update the value of a
    for(int i = 0;i < features.size();i++){
      if(mistakes[i]){
	float feature_magnitude = 0;
	for(int j = 0;j < DIM;j++){
	  feature_magnitude += features[i][j]*features[i][j];
	}
	for(int j = 0;j < DIM;j++){
	  a[j] += (eta[j]*(margin - dotprod[i])*features[i][j])/((feature_magnitude));
	}
      }
    }
    for(int j = 0;j < DIM;j++){
      // eta[j] = 1/(float)loop_count;//update value of eta
      cout<<a[j]<<" ";
    }
    cout<<endl;
    if(loop_count > 100){
      cout<<"approximate solution\n";
      break;
    }
  }
  return 0;
}

int BatchPerceptron(vector<vector<float> >& features, vector<float>& a,float margin ){
  for(int i = 0;i < DIM;i++){
    a.push_back(rand()%7);
  }
  float eta[DIM] = {0.1,0.1,0.1};
  float dotprod = 0;
  int mistake_count = 0,loop_count = 0;
  bool* mistakes = new bool[features.size()];
  while(1){
    loop_count++;
    for(int i = 0;i < features.size();i++){
      //compute dot product
      for(int j = 0;j < DIM;j++){
	dotprod += a[j]*features[i][j]; 
      }
      //find mistakes;
      if(dotprod < margin){
	mistakes[i] = true;
	mistake_count++;
      }
      else mistakes[i] = false;
      //set dotprod to 0 and start with another vector;
      dotprod = 0;
    }
    if(mistake_count < 1) break;//
    //cout <<mistake_count<<" mistakes \n";
    mistake_count = 0;
    //update the value of a
    for(int i = 0;i < features.size(); i++){
      if(mistakes[i]){
	for(int k = 0 ;k < DIM;k++){
	  a[k] += features[i][k]*eta[k];
	}
      }
    }
    if(loop_count > 5000) break;//bahut ho gaya ab na ho paye!
  }
  delete[] mistakes;
  return 0;
}

int SingleSamplePerceptron(vector< vector<float> >& features,vector<float>& a,float margin){
  for(int i = 0;i < DIM;i++){
    a.push_back(rand()%7);
    //cout << a[i]<<endl;
  }
  float eta[DIM] = {1,1,1};
  float dotprod = 0;
  int count = 0;
  for(int i = 0;i < features.size();i++){
    count++;
    if(count > 1000000){
      cout << "approximate solution"<<endl;
      break;
    }
    //compute dot product   
    dotprod = 0;
    for(int j = 0;j < DIM;j++){
      dotprod += a[j]*features[i][j];
    }
    //wrong classification
    if(dotprod < margin){ 
      //update a vector
      // cout << dotprod<<endl;
      for(int j = 0;j < DIM;j++){
	a[j] += features[i][j]* eta[j];
      }
      i = 0;
    }
  }

  return 0; 
}

int ReadData(vector< vector<float> > &features, vector<int> &labels,const char*data_filename, const char *labels_filename){
  ifstream  data(data_filename);
  ifstream lbls(labels_filename);
  float x,y;
  char temp;
  int l,i = 0;
  if(data.is_open()){
    while(data >> x >> temp >> y){
      //  cout << x<<" "<<y<<endl;
      features.push_back(vector<float>());//
      features[i].push_back(1);
      features[i].push_back(x);
      features[i].push_back(y);
      i++;
    }
  } 
  else{
    cout<<"error reading data file"<<endl;
    exit(-1);
  }
  if(lbls.is_open()){
    while(lbls>>l){
      labels.push_back(l);
    }
  }
  else{
    cout<<"error reading label files"<<endl;
    exit(-2);
  }
  return 0;
}

/*
int plot_points(Mat& plot,vector< vector<float> >&points, vector<int>& color){
  int origin  = 30;
  line(plot,
       Point(origin,0),
       Point(origin,plot.cols),
       Scalar(255,0,0),
       1);
  line(plot,
       Point(0,plot.rows - origin),
       Point(plot.cols,plot.rows- origin),
       Scalar(255,0,0),
       1);
  for(int i = 0 ;i < points.size();i++){
    circle(plot,
	   Point(points[i][1] + origin, plot.rows - points[i][2] - origin),//center
	   2,//radius
	   Scalar(0,255,0),//color
	   -1,//thickness
	   8);//connectivity
  }
  namedWindow("--Plot--",CV_WINDOW_NORMAL);
  imshow("--Plot--",plot);
  waitKey(0);
  return 0;
}
*/
