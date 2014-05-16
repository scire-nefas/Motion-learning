/* Copyright 2013 Syed Tabish, Palash Pandya. All rights reserved.    

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
#include"functions.h"

using namespace std;
using namespace cv;




int main(int argc, char* argv[]){
	Mat Image;
	vector<Mat> data, testData;
	vector<string> label, testLabelstr;
	ofstream file;
	file.open("TESTSVM4.txt");
	//PARSING TEST
	int r = 12;
	cout<<"PARSING TEST"<<endl<<endl;
	vector<bool> randv(r, true); 
	GenerateRandv(randv, r);
	string dirdir = "students";
	ParseData(data, label, testData, testLabelstr, dirdir, randv);

	//PCA TEST
	cout<<endl<<"PCA TEST"<<endl<<endl;

	PCA pca_object;
	cout<<"Eigenbasis\n";
	ComputeEigenBasis(pca_object,50,data);
	//	SaveData(pca_object,randv);
	//	LoadData(pca_object,randv);

/*************************************************************************************************
 * 			K-NN TESTBENCH								 *
 ************************************************************************************************/
	
	/*
	cout<<"Entering test bench\n";
	for(int i=10;i<55;i+=10){

		Mat trainData, trainLabel, testFeatures, testLabel;
		TestBench(trainData,trainLabel,pca_object, data, label, i);
		TestBench(testFeatures,testLabel,pca_object, testData, testLabelstr, i);

		for(int j = 1;j<12;j+=2){

			cout<<"\n\nExperiment for EigCount="<<i<<", K="<<j<<".\n\n";
			file<<i<<" "<<j<<endl;
			CvKNearest knnMod;
			float lbl; int k=j;
			Mat testImage;
			Mat currentLabel, currentTest;
			KnnTrain(knnMod, trainData, trainLabel);
			//for each testFeature row, run knnClassify and report accuracy.
			int num = testFeatures.rows, correct=0;
			for(int i=0;i<num;i++){
				currentTest = testFeatures.row(i);
				KnnClassify(lbl, knnMod, currentTest, k);
				cout<<"Obtained label:"<<lbl<<"and actualLabel:"<<testLabel.at<float>(i,0)<<endl;

				if(testLabel.at<float>(i,0)==lbl){
					correct++;
				}
			}
			file<<(((float)correct/(float)num)*100)<<endl;
		}
	}
	*/
/************************************************************************************************
*************************************************************************************************/


/*************************************************************************************************
 * 			SVM TESTBENCH								 *
 ************************************************************************************************/
	
	for(int i=20;i<55;i+=10){
		Mat trainData, trainLabel, testFeatures, testLabel,predictedLabel;
		float acc;
		TestBench( trainData, trainLabel, pca_object, data, label, i);
		TestBench( testFeatures, testLabel, pca_object, testData, testLabelstr, i);
		CvSVM svm;
		TrainSvm( svm, trainData, trainLabel);
		TestSvm( svm, testFeatures, predictedLabel);
		Accuracy( acc, testLabel, predictedLabel);
		cout<<"\n\nEigCount="<<i<<"; Accuracy="<<acc<<" !\n\n";
		file<<i<<" "<<acc<<endl;

	}


	file.close();
	return 0;
}


