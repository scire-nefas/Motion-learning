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
#include<math.h>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<ctime>

using namespace std;

#define LEN 784
#define DIM 28*2

int single = 0;

int ReadData(const char*, const char*, vector<vector<float> >&, vector<int>&);
int OneOnePerceptron(vector<vector<float> >&,vector<vector<float> >&,vector<int>&,float);
int OneRestPerceptron(vector<vector<float> >&,vector<vector<float> >&,vector<int>&,float);
int ddag(vector<vector<float> >&,vector<float>&);
int voting(vector<vector<float> >&,vector<float>&);


int main(int argc, char* argv[]){
  const char* trdatafile = "MnistTrainData.txt";
  const char* trlabelfile = "MnistTrainLabel.txt";
  const char* tedatafile = "MnistTestData.txt";
  const char* telabelfile = "MnistTestLabel.txt";

  vector<vector<float> > TrainingFeatures;
  vector<int> TrainingLabels;
  
  vector<vector<float> > TestFeature;
  vector<int> TestLabels;
  
  vector<vector<float> > one_one_classifiers;
  vector<vector<float> > one_rest_classifiers;

  ReadData(trdatafile,trlabelfile,TrainingFeatures,TrainingLabels);
  ReadData(tedatafile,telabelfile,TestFeature,TestLabels);
  cout<<" Enter choice:\n";
  cout<<"1. One-One Perceptron using DDAG without margin\n";
  cout<<"2. One-rest Perceptron using (scaled)majority vote\n";
  cout<<"3. One-One Perceptron using DDAG without margin\n";
  cout<<"4. One-rest Perceptron with margin using majority vote\n";
  cout<<"5. One-One Single Perceptron with DDAG\n";
  int choice = 2;
  cin >>choice;
  srand((unsigned)time(0));
  switch(choice){
  case 1:{
    OneOnePerceptron(one_one_classifiers,TrainingFeatures,TrainingLabels,0);
    float error = 0;
    for(int u = 0;u < TestFeature.size();u++){
      int cat = ddag(one_one_classifiers,TestFeature[u]);
       cout<<"actual "<<TestLabels[u]<<" mine->"<<cat<<endl;
      if(cat != TestLabels[u]) error++;
    }
    cout<<"error percentage - > "<<error/1014;
    break;
  }
  case 2:{
    OneRestPerceptron(one_rest_classifiers,TrainingFeatures,TrainingLabels,0);
    float error = 0;
    for(int u = 0;u < TestFeature.size();u++){
      int cat = voting(one_rest_classifiers,TestFeature[u]);
      //     cout<<"actual "<<TestLabels[u]<<" mine->"<<cat<<endl;
      if(cat != TestLabels[u]) error++;
    }
    cout<<"error percentage -> "<<error/1014;
    break;
  }
  case 3:{
    float margin = 0;
    cout<<"Enter margin: \n";
    cin >> margin;
    OneOnePerceptron(one_one_classifiers,TrainingFeatures,TrainingLabels,margin);
    float error = 0;
    for(int u = 0;u < TestFeature.size();u++){
      int cat = ddag(one_one_classifiers,TestFeature[u]);
      // cout<<"actual "<<TestLabels[u]<<" mine->"<<cat<<endl;
      if(cat != TestLabels[u]) error++;
    }
    cout<<"error percentage - > "<<error/1014;
    break;
  }
 case 4:{
   float margin = 0;
   cout<<"Enter Margin: \n";
   cin>>margin;
    OneRestPerceptron(one_rest_classifiers,TrainingFeatures,TrainingLabels,margin);
    float error = 0;
    for(int u = 0;u < TestFeature.size();u++){
      int cat = voting(one_rest_classifiers,TestFeature[u]);
      // cout<<"actual "<<TestLabels[u]<<" mine->"<<cat<<endl;
      if(cat != TestLabels[u]) error++;
    }
    cout<<"error percentage -> "<<error/1014;
    break;
  }
 case 5:{
   single = 1;
    OneOnePerceptron(one_one_classifiers,TrainingFeatures,TrainingLabels,0);
    float error = 0;
    for(int u = 0;u < TestFeature.size();u++){
      int cat = ddag(one_one_classifiers,TestFeature[u]);
//       cout<<"actual "<<TestLabels[u]<<" mine->"<<cat<<endl;
      if(cat != TestLabels[u]) error++;
    }
    cout<<"error percentage - > "<<error/1014;
    break;
  }
  }
  return 0;
}

int voting(vector<vector<float> >&classifiers,vector<float>&sample){
  int c = 0;
  int category = -1;
  float vote[10] = {0};
  for(int i = 0;i < 9;i++){
    float dotprod = 0;
    for(int j = 0;j < DIM;j++)
	dotprod += classifiers[i][j]*sample[j];
    if(dotprod > 0){
      vote[i] -= dotprod;
    }  
  }
    /* for(int i = 0;i< 10;i++ ){
      cout<<vote[i]<<" ";
      }
      cout<<endl;*/
    int  min = 0;
    for(c = 0;c < 9;c++){
      if(vote[c] < min){
	min = vote[c];
	category = c;
      }
    }
    return category;
}
  
int ddag(vector<vector<float> >& classifiers,vector<float>& sample){
  int c1 = 0,c2 = 9;
  int category = -1;
  for(int i = 0; i < 9;i++){
    int cls = 8*c1 + c2 -((c1*(c1-1))/2) - 1;
    float dotprod = 0;
    //compute dot product;
    for(int j = 0;j <= DIM;j++){
      dotprod += classifiers[cls][j]*sample[j];
    }
    if(dotprod > 0){
      category = c1;
      c2--;
    }else{
      category = c2;
      c1++;
    }
    // cout<<"dot prod->-"<<dotprod<<endl;
  }
  return category;
}

int OneRestPerceptron(vector<vector<float> >&a,vector<vector<float> >&features,vector<int>&labels,float margin){
  int cls = -1;
  for(int c = 0;c<=9;c++){
     int loop_count = 0;
     float dotprod = 0;
     float eta[DIM+1];for(int k = 0;k <= DIM;k++) eta[k] = 0.1;
     //initialize a vector
     a.push_back(vector<float>());
     for(int k = 0;k <= DIM;k++){
       a[c].push_back((float)rand()/(float)RAND_MAX);
     }
     a[c].push_back((float)c);//name of classifier
     //*****************************************
     for(int i = 0;i < features.size();i++){
        if(labels[i] != c)continue;//others are irrelevant and may be misleading.....
	loop_count++;if(loop_count > 10000)break;//ab na ho paye!
	//compute dotproduct
	for(int j = 0;j <= DIM;j++){
	  dotprod += a[c][j]*features[i][j];
	}
	//	cout<<dotprod<<" -->> ";
	//check for misclassification
	if(dotprod < margin){
	  //update a vector
	  for(int j = 0;j <= DIM;j++){
	    a[c][j] += eta[j]*features[i][j];
	  }
	}
	dotprod = 0;
     }
     /*
     for(int k  = 0;k <=DIM;k++){
       cout<<a[c][k]<<" ";
     }
     cout<<"<"<<c<<">"<<endl;
     */
  }
  return 0;
}

int OneOnePerceptron(vector<vector<float> >&a,vector<vector<float> >&features,vector<int>&labels,float margin){
  int cls = -1; 
  for(int c1 = 0;c1 <= 9;c1++){//for each classifier
    for(int c2 = c1+1 ;c2 <= 9;c2++){
      bool *mistakes = new bool[features.size()];
      vector<float> temp(DIM+1,0);
      int loop_count = 0,mistake_count = 0,min_mistakes = 100000;
      float dotprod = 0;
      int category = 1;
      float eta[DIM+1];
      for(int k = 0;k <= DIM;k++) eta[k] = 1;
      a.push_back(vector<float>());
      //initialize the a vector
      cls++;for(int k = 0;k <= DIM;k++) a[cls].push_back((float)rand()/(float)RAND_MAX);
      
      //name of the classifier
      a[cls].push_back((float)c1);
      a[cls].push_back((float)c2);
      while(1){
          loop_count++;
	  if(loop_count > 1000){
	    //    cout<< "stoped at "<<loop_count<<endl;
	    break;//ab na ho paye....
	  }
	  for(int i = 0;i < features.size();i++){
	     if((labels[i]) != c1 && (labels[i] != c2)) continue;//choose only the two classes
	     if(labels[i] == c2) category = -1;//invert the -ve class
	     
	     //compute dot product
	     for(int j = 0;j <= DIM;j++){
	       dotprod += features[i][j]*a[cls][j];
	     }
	     dotprod *= category;
	     category = 1;
	     //check for missclassification
	     if(dotprod < margin){
	       mistakes[i] = true;
	       mistake_count++;
	     }else{
	       mistakes[i] = false;
	     }
	  
	  }
	  //save the best till now
	  if(mistake_count < min_mistakes){
	    min_mistakes = mistake_count;
	    for(int j = 0;j<=DIM;j++){
	      temp[j] = a[cls][j];
	    }
	  }
	  if(!mistake_count)break;
	  // cout<<"Total mistakes->"<<mistake_count<<endl;
	  mistake_count = 0;
	  //update a vector
	  for(int i = 0;i<features.size();i++){
	    if(!mistakes[i])continue;
	    for(int j = 0;j <= DIM;j++){
	      a[cls][j] += eta[j]*features[i][j];
	      //  cout<<eta[j]<<" * "<<features[i][j]<<"+="<<a[cls][j]<<" ";
	      if(single) break;
	    }
	    // cout<<endl;
	  }
	  // for(int k = 0;k<DIM;k++){
	  // cout<< a[cls][k]<<" ";
	  //}

      }
      //make a equal to the best classifier
      for(int j = 0;j<=DIM;j++){
	a[cls][j] = temp[j];
      }
      /*   //print out the a vector;
      cout<<"min mistakes->"<<min_mistakes<<endl;
      for(int k = 0;k<=DIM;k++){
	cout<< a[cls][k]<<" ";
      }
      cout<<"< " <<c1<<","<<c2<<">"<<cls<<endl;
      */
      delete[] mistakes;
    }
  }
  
  return 0;
}

int ReadData(const char* data_filename, const char* label_filename, vector<vector<float> >& features, vector<int>&labels){
  ifstream datafile(data_filename);
  ifstream labelfile(label_filename);
  float* image = new float[LEN];
  int count = 0;
  float total = 0;
  if(datafile.is_open()){
    while(datafile >> image[0]){
      //read the image into array image
      for(int j = 1;j < LEN;j++){
	datafile>>image[j];
	if(image[j] > 5)image[j] = 1;
	else image[j] = 0;
      }
      
      //if(!count%4)continue;
      //compute vertical projection
      vector<float> hor(DIM/2,0);
      vector<float> ver((DIM/2)+1,0);
      for(int j = 0;j < LEN;j++){
	ver[j/(DIM/2)] += image[j];
	hor[j%(DIM/2)] += image[j];
	total += image[j];
      }
      for(int j = 0;j < DIM/2;j++){
		ver[j] /= total;
		hor[j] /= total;
	//	cout<<ver[j]<<" ";
      }
      // cout<<endl;
      total = 0;
      ver[DIM/2] = 1;//augumentation
      features.push_back(vector<float>());
      features[count].insert(features[count].end(),hor.begin(),hor.end());
      features[count].insert(features[count].end(),ver.begin(),ver.end());
      count++;
    }
    cout<<count<<"images read \n";
  }else{
    cout<<"unable to read data file"<<endl;
    exit(-1);
  }

  ///////////////////////////
  if(labelfile.is_open()){
    int x;
    while(labelfile>>x){
      labels.push_back(x);
    }
  }else{
    cout<<"unable to read label file"<<endl;
    exit(-2);
  }
}

