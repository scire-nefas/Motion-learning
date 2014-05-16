#include<iostream>

using namespace std;

int select(int* array, int start, int end, int k);

int main(){
    int n,k;
    int *array;
    //read data
    cin >> n >> k;
    array = new int[n];
    for(int i = 0; i < n; i++){
        cin >> array[i];
    }

    //algorithm

    cout << select(array,0,n-1,k) << endl;
    return 0;
}

int select(int* array, int start, int end, int k){
    //for randomizing put code to swap random element with array[end]
    if(start > end){
        return 0;
    }
    int pivot = array[end];
    int j = start;
    for(int i = start;i <= end; i++){
        if(array[i] <= pivot){
	    int temp = array[j];
	    array[j] = array[i];
	    array[i] = temp;
	    j++;
	}
    }
//    for(int i = start;i <= end; i++){
//        cout<<array[i]<<" ";
//    }
//    cout<<" -> "<<k<<"th( "<<j<<")"<<endl;
//    return 1;
    if(j - start == k){
        return array[j-1];
    }
    else if(j - start < k){
        return select(array,j,end,k-j+start);
    }
    else if(j - start > k){
	return select(array,start,j-2,k);
    }
    return -1;
}
