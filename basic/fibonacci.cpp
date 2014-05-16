#include<iostream>

using namespace std;

int* fibonacci(int* mat, int n);

int main(){
    int n;
    int mat[] = {1,1,1,0};
    int* m = &mat[0];
    //read data
    cin >> n;
    if(n < 1){
       cout << 0;
       return 0;
    }
    int* pow_mat = fibonacci(m,n);
    cout << pow_mat[2] << endl;
    return 0;
}

int* fibonacci(int* mat, int n){
    if(n == 1){
      return mat;
    }
    int *half = fibonacci(mat, n/2);
    int *full = new int[4];
    full[0] = half[0]*half[0] + half[1]*half[2];
    full[1] = half[0]*half[1] + half[1]*half[3];
    full[2] = half[2]*half[0] + half[3]*half[2];
    full[3] = half[2]*half[1] + half[3]*half[3];
    if(n & 1){
	int temp = full[0];
        full[0] = full[0] + full[1];
	full[1] = temp;
	temp = full[2];
	full[2] = full[2] + full[3];
	full[3] = temp;
	return full;

    }
    else{
        return full;
    }
    
}
