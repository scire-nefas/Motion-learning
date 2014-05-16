#include<iostream>

using namespace std;

int* sort(int *array, int N);

int main(){
    int N;
    int *array;
    //read data
    cin >> N;
    array = new int[N];//allocate space
    for(int i = 0;i < N;i++){
        cin >> array[i];
    }
    //start algorithm
    array = sort(array,N);
    //print output
    for(int i = 0;i < N;i++){
        cout << array[i] <<" ";
    }
    cout<<endl;
    return 0;
}

int* sort(int *array, int N){
    //base case
    if(N == 1){
//        cout<<*array<<endl;//see each step
    	return array;
    }
    //general case
    int* left = sort(array, N/2);
    int* right = sort(array+ N/2, N - N/2 );
    int* sorted = new int[N];
    int l = 0, r = 0, i = 0;
    while(l + r < N){
      while(((left[l] <= right[r]) || (r >= (N - N/2))) && (l < N/2)){
          sorted[i] = left[l];
	  l++; i++;
      }
      while( ((right[r] <= left[l]) || (l >= N/2)) && (r < (N - N/2)) ){
          sorted[i] = right[r];
	  r++; i++;
      }
    }
    /*//see each step
    for(int k = 0;k < N;k++){
        cout<<sorted[k]<<" ";
    }
    cout<<endl;
    */
    return sorted;
}

/*
    Running time of algo is theta(nlogn)
    T(n) = { theta(1) when n=1 //compare n and return
           { 2*T(n/2) + theta(n) // sort each half(T(n/2)) and merge(theta(n))
    solve recurence:
    T(n) = theta( n*log_2(n) )
*/
