/* Copyright 2013 Syed Tabish, Palash Pandya . All rights reserved.    

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
#include"functions.hpp"

int ReadImage(Mat& Image, string filename, int flag){
	Mat tempimage = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	if(!tempimage.data){
		cerr<< "Could not open image file"<<endl;
		return 1;
	}
	DetectFace(tempimage,Image,flag);
	PreProcess(Image);
	if(!Image.isContinuous()){
	//	cerr<<"Image not continous\n";
		return 1;
	}
	Image = Image.reshape(0,1);

	return 0;  
}

int DetectFace(Mat& tempimage, Mat& Img, int flag){
	vector<Rect> Faces;

	if(flag == DONT_DETECT_FACE){
		resize(tempimage,Img,Size(ROWS,COLS));
		return 0;
	}

	string DetectorName = "haarcascade_frontalface_alt.xml";
	CascadeClassifier FaceDetector;
	if(!FaceDetector.load(DetectorName)){
	//	cerr<<"Error Loading Face Detector"<<endl;
		return 1;
	}
	//detect faces in the image
	FaceDetector.detectMultiScale(tempimage,//input image
			Faces,//vector to store detected regions
			1.1,//change in scale for level change
			2,//neighbours(don't know what it means)
			CV_HAAR_SCALE_IMAGE,//using haar cascade
			Size(70,70));//minimum size of image
	//handling case where detector fails
	if(Faces.size() == 0){
	//	cerr<<"No Faces were detected in the image"<<endl;
	//	cout<<"Starting recognition with unknown image.... "<<endl;
		resize(tempimage,Img,Size(ROWS,COLS));
		return 1;
	}
	//returning only the first detected(probably largest) image
	Img = tempimage(Faces[0]);
	return 0;
}

int PreProcess(Mat& Image){
	// Blur the image to remove noise
	GaussianBlur(Image,//source image
			Image,//destination image
			Size(5,5),//size of kernel
			1.2);//sigma
//	double gamma = 1/(0.6);
	//Image.convertTo(Image,CV_32F);
	//pow(Image,gamma,Image);
	//double min,max;
	//minMaxLoc(Image,&min,&max);
	//Image.convertTo(Image,CV_8U,255.0/(max-min),-255.0/min);
	//improve contrast.
	equalizeHist(Image,Image);
	return 0;
}

int GenerateRandv(vector<bool> & randv, int  n){
	for(int i=0;i<n/4;i++){
		int idx;
		randv[(idx = rand()% n + 1)]=false;
		cout<<idx<<" : "<<randv[idx]<<" | ";
	}
	cout<<endl;
	return 0;
}

int getSubDirnames(DIR* dirp, vector<string> & subdir){
	struct dirent * dp;
	while((dp = readdir(dirp))!=NULL){
		if(*(dp->d_name)=='.' )
			continue;
		subdir.push_back(dp->d_name);
		cout<<dp->d_name<<endl;
	}
	return 0;
}

int ParseData(vector<Mat> & data, vector<string> & label, vector<Mat> & testData, vector<string> & testLabelstr, string & dirname, vector<bool> & randv){
	Mat Image;
	DIR * dirp;
	dirp = opendir(dirname.c_str());
	vector<string> subdir;
	getSubDirnames(dirp, subdir);
	//labels are same as the subdirnames..
	for(unsigned i=0; i<subdir.size();i++){
//		if(randv[i]){
			label.push_back(subdir[i]);
			testLabelstr.push_back(subdir[i]);
			//parse this subdir
			//string::const_pointer a = &subdir[i];
			string newdir = dirname+"/"+subdir[i];
			DIR* sdir = opendir(newdir.c_str());
			struct dirent * sdp;
			string name;
			int cnt=0,k=0;
			data.push_back(Mat());
			testData.push_back(Mat());
			while((sdp = readdir(sdir))!=NULL){
				k++;
				
				name = newdir+"/" + sdp->d_name;
	//			cout<<"Reading "<<name<<endl;
				int readstatus = ReadImage(Image,name,DETECT_FACE);
				if(readstatus ==1){
					continue;
				}
				cnt++;
				if(randv[k]){
					data[i].push_back(Image);
				}else{
					testData[i].push_back(Image);
				}
			}
			cout<<" reading "<<cnt<<" files"<<endl;
			(void)closedir(sdir);
	//	}else{
/*			testLabelstr.push_back(subdir[i]);

			//parse this subdir
			//string::const_pointer a = &subdir[i];
			string newdir = dirname+"/"+subdir[i];
			DIR* sdir = opendir(newdir.c_str());
			struct dirent * sdp;
			string name;
			int cnt=0;
			testData.push_back(Mat());
			while((sdp = readdir(sdir))!=NULL){
				name = newdir+"/" + sdp->d_name;
	//			cout<<"Reading "<<name<<endl;
				int readstatus = ReadImage(Image,name,DETECT_FACE);
				if(readstatus ==1){
					continue;
				}
				cnt++;
				testData[i].push_back(Image);
			}
			cout<<" read "<<cnt<<" files"<<endl;
			(void)closedir(sdir);		
*/	//	}
	}

	(void) closedir(dirp);
	return 0;
}


int ComputeCoefficients(Mat& feature,PCA& pca_object,Mat& image){
	if(!image.data){
		cout<<"Input image passed for computing coeffecients seems to be empty\n";
		return 1;
	}
	image = image.reshape(0,1);//convert image into row;
	feature = pca_object.project(image);
	//put check for this step 
	//unlikely to fail coz data comes form Read Image
	//note nothing changes if the test fails only returned 
	//value changes.. 
	if(image.cols != COLS*ROWS){
		cout<<"The size of the image vector is inappropriate\n";
		return 2;
	}

	return 0;
}

int ComputeEigenBasis(PCA& pca_object,int maxComponents, vector<Mat>& data){
	int classcount = data.size();
	Mat A;
	//concatenate all data into single matrix
	for(int i = 0; i  < classcount; i++){
		A.push_back(data[i]);
	}
	cout<<"size of Training Data of Eigen vector computation : "<<A.size()<<endl;
	pca_object(A,Mat(),CV_PCA_DATA_AS_ROW,maxComponents);


	/*debug code
	  eigenBasis = pca_object.eigenvectors; 
	  cout<<"size of eigen basis: " << eigenBasis.size()<<endl;
	  namedWindow("Disp",CV_WINDOW_NORMAL);
	  Mat proj = pca_object.project(A.row(0));
	  cout<<"size of projection:"<<proj.size()<<endl;
	  Mat reconImage = pca_object.backProject(proj);
	  cout<<"data type of reconstructed image: "<<reconImage.depth()<<" "<<CV_32F<<endl;
	  double min,max;
	  minMaxLoc(reconImage,&min,&max);
	  reconImage.convertTo(reconImage,CV_8U,255.0/(max-min),-255.0/min);
	  imshow("Disp",reconImage.reshape(0,80));
	  waitKey(0);
	  imshow("Disp",A.row(0).reshape(0,80));
	  waitKey(0);
	  */  

	return 0;
}


int SaveData(PCA & pca_object, vector<bool> & randv){
	Mat eig = pca_object.eigenvectors;
	Mat eval = pca_object.eigenvalues;
	Mat mean = pca_object.mean;
	FileStorage file1("eigenbasis.xml", 1);
	file1<<"randv"<<"[";
	for(unsigned i=0;i<randv.size();i++){
		file1<<randv[i];
	}
	file1<<"]";
	file1<<"EigenVectors"<<eig;
	file1<<"EigenValues"<<eval;
	file1<<"Mean"<<mean;
	return 0;
}

int LoadData(PCA & pca_object, vector<bool> randv){
	FileStorage file1("eigenbasis.xml",0);
//	file1["randv"]>>randv;
	file1["EigenVectors"]>>pca_object.eigenvectors;
	file1["EigenValues"]>>pca_object.eigenvalues;
	file1["Mean"]>>pca_object.mean;
	return 0;	
}

int ExtractFeatures(Mat & trainData, Mat & trainLabel, vector<Mat> & data, vector<string> & label, PCA& pca_object){
	for(int i=0;i<(int)data.size();i++){
		int dataRow = data[i].rows;
		//int dataCol = data[i].cols;
		//Extract the class number (not more than 100 classes)
		string lab = label[i]; 
		int len = lab.size();
		float classNum;
		if(len>=2){
			classNum = lab[len-1]-'0' + (lab[len-2]-'0')*10;
		}else{
			classNum = lab[len-1]-'0';
		}
		cout<<"Extracting features for the class "<<lab<<"with classNum "<<classNum<<endl;
		Mat A = Mat::ones(dataRow,1,CV_32F)*classNum;
		trainLabel.push_back(A);

		cout<<"Rows:"<<trainLabel.rows<<" Cols:"<<trainLabel.cols<<endl;
		//Then for each row of data[i]
		//Calculate coefficients.
		//push_back on train data, and class number in eigenbasis.
		Mat feature, image; //The feature vector that will be returned by computeCoefficient.
		for(int j=0;j<dataRow;j++){
			image = data[i].row(j);
			ComputeCoefficients(feature, pca_object, image);
			trainData.push_back(feature);
		}

	}
	return 0;
}

int TestBench(Mat & finalData, Mat & finalLabel, PCA& pca, vector<Mat> & data, vector<string> & label, int vectorcount){
	PCA temp_pca;
	Mat eigen = pca.eigenvectors;
	Mat vals = pca.eigenvalues;
	Mat mean = pca.mean;
	temp_pca.eigenvectors = eigen.rowRange(0,vectorcount);
	temp_pca.eigenvalues = vals.rowRange(0,vectorcount);//doesnt matter
	temp_pca.mean = mean;

	ExtractFeatures(finalData, finalLabel, data, label, temp_pca);
	/*

	   for(int i = 0;i < data.rows;i++){
	   coeff.push_back(temp_pca.project(data.row(i)));
	   }
	   */
	return 0;
}

int KnnTrain(CvKNearest & knnMod, Mat & trainData, Mat & trainLabel){
	//First train the classifier on the the trainData
	knnMod.train(trainData, trainLabel);
	return 0;
}

int KnnClassify(float & label, CvKNearest & knnMod, Mat & testImage, int & k){
	//Classify the testImage and return class label
	cout<<"Classification now \n\n";
	label = knnMod.find_nearest(testImage, k);
	return 0;
}

int Accuracy(float& accuracy, Mat& testLabel, Mat& predictedLabel){
	float correct;
	for(int i = 0; i < testLabel.rows; i++){  
		if(predictedLabel.at<float>(i,0) == testLabel.at<float>(i,0)){
			correct++;
		}
	}

	accuracy = (float) 100 * (float)correct / (float )testLabel.rows;
	cout<<"accuracy of prediction is: "<<accuracy<<"% "<<endl;
	return 0;
}

int TestSvm(CvSVM& svm, Mat& testData, Mat& testLabel){
	int correct;
	for(int i = 0; i < testData.rows; i++){
		float response =  svm.predict(testData.row(i));

		testLabel.push_back(response);
	}
	return 0;
}
int TrainSvm(CvSVM& svm, Mat& trainData, Mat& trainLabel){
	CvSVMParams svmParams;
	svmParams.svm_type    = CvSVM::C_SVC;
	svmParams.kernel_type = CvSVM::POLY;
	svmParams.degree = 5;
	//svmParams.gamma = ?;
	svmParams.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	int k_fold = 4;

	 svm.train(trainData,trainLabel,Mat(),Mat(),svmParams);

//	svm.train_auto(trainData,trainLabel,Mat(),Mat(),svmParams,k_fold);
	return 0;


}
