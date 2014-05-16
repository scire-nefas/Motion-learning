#include<iostream>

#define MAX 100

using namespace std;

int main(){
    int n;
    int *a,*b;
    int c[MAX] = {0};
    //read data
    cin >> n;
    a = new int[n];
    b = new int[n];
    for(int i = 0; i < n;i++){
        cin >> a[i];
    }
    //algorithm
    //ensure c is zero
    for(int i = 0;i < n; i++){
        c[a[i]]++;
    }
    for(int i = 1;i < MAX;i++){
      c[i] += c[i-1];
    }
    for(int i = n-1;i >= 0;i--){
      b[c[a[i]]-1] = a[i];
      c[a[i]]--;
    }
    //output data
    for(int i = 0; i < n;i++){
        cout << b[i] << " ";
    }
    cout << endl;
    return 0;
}

/*
    Running time is theta(k + n)
    k is the MAX int value of array
     
*/
