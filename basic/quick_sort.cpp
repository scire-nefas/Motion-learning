#include<iostream>

using namespace std;

int quicksort(int* array, int start, int end);

int main(){
    int n;
    int *array;
    //read data
    cin >> n;
    array = new int[n];
    for(int i = 0; i < n; i++){
        cin >> array[i];
    }
    //algorithm
    quicksort(array,0,n-1);
    //print output
    for(int i = 0;i < n;i++){
        cout << array[i] << " "; 
    }
    cout << endl;
    return 0;
}

int quicksort(int* array, int start, int end){
    if(start >= end) return 0;
    int pivot = array[end];
    int i = start;
    for(int j = start ; j <= end; j++){
        if( array[j] <= pivot ){
	    int temp  = array[i];
	    array[i] = array[j];
	    array[j] = temp;
	    i++;
	}
    }
    --i;
    quicksort(array,start,i-1);
    quicksort(array,i+1,end);
    return 0;
}
