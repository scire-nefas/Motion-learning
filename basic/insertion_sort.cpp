#include<iostream>
using namespace std;

int main(){
    int N;
    int *array;
    //read data
    cin>>N;
    array = new int[N];//allocate sapce
    for(int i = 0;i < N;i++){
        cin >>  array[i];
    }
    //start of algorithm
    for(int i = 1;i < N;i++){
        int key = array[i];
	int j = i-1;
	while( (j >= 0) && (array[j] > key) ){
	    array[j + 1] = array[j];
	    --j;
	}
	array[j + 1] = key;
    }
    //print out result
    for(int i = 0;i < N;i++){
        cout<<array[i]<<" ";
    }
    cout<<endl;
    return 0;
}

/*
   Running time analysis
   Dependance on input data:
   1. If the data is sorted there is not much work 
      do as porgram will not go in the while loop
      just run through the data once.
   2. Worst case is when the data is sorted in reverse
      order. In that case every time there have to be
      i(1,2,...,n) swaps in the insertion step.
   Dependence on size of input data:
   The algo is theta(n^2)
      // we do theta(i) work in every iter. of loop
      // this theta(i) is eg C*i for worst case(i swaps);
      T(n) = \sum_i {theta(i)};
           = theta(n^2);
      
 */
