#include<iostream>

using namespace std;

bool search(int* array, int N, int key);

int main(){
    int N,key;
    int* array;
    //read data
    cin >> N;
    array = new int[N];
    for(int i = 0;i < N;i ++ ){
        cin >> array[i];
    }
    cin >> key;
    if(search(array,N,key)){
        cout << "found"<<endl;
    }
    else{
        cout << "not found"<<endl;
    }
    return 0;
}
bool search(int* array, int N, int key){
    if(array[N/2] == key){
        return true;
    }
    if(N == 1){
       return false;
    }
    else if(array[N/2] < key){
        return search(array+N/2,N - N/2,key);
    }
    else if(array[N/2] > key){
        return search(array, N/2,key);
    }
}

/*
    Running time is theta(log_2(n))
    T(n) = T(n/2) + theat(1); 
*/
