#include<iostream>

using namespace std;

int main(){
    int n;
    int *a,*b,*t;
    int bits = 8;
    int mask = 1;
    for(int i = 0;i  < bits;i++){
     mask |= (1 << i); 
    }
    int c[256] = {0};
    //read data
    cin >> n;
    a = new int[n];
    b = new int[n];
    t = new int[n];
    for(int i = 0; i < n; i++){
        cin >> a[i];
	t[i] = a[i];
    }
    //algorithm
    for(int j = 1; j <= 4;j++){
        for(int i = 0;i < 256;i++){
	    c[i] = 0;
	}
        for(int i = 0;i < n;i++){
	    c[(t[i] & mask)]++;
	}
	for(int i = 1;i < 256;i++){
	    c[i] += c[i-1];
	}
	for(int i = n-1;i >= 0;i--){
	    b[c[(t[i] & mask)] - 1] = a[i];
	    c[(t[i] & mask)]--;
	}
	for(int i = 0;i < n;i++){
	    t[i] = (b[i] >> j);
	}
	int *temp = b;
	b = a;
	a = temp;
    }

    //print out data
    for(int i = 0;i < n;i++){
        cout << a[i] << " ";
    }
    cout << endl;
    return 0;
}
