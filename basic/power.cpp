#include<iostream>

using namespace std;

int power(int x , int n);

int main(){
    int n,x;
    //read data
    cin >> x >> n;
    cout << power(x,n)<<endl;
    return 0;
}

int power(int x , int n){
  if(n == 1){
    return x;
  }
  if(n & 1){//odd
      int half = power(x,(n-1) >> 1);
      return half * half * x;
  }
  else{
    int half = power(x,n>>1);
    return half * half;
  }
}

/*
  Time complexity is theta(log_2(n))
  T(n)  = T(n/2) + theta(1)
*/
