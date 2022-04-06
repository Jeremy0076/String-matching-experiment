#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#include<ctime>
#include<cstdlib>
#define ll long long
using namespace std;

const int dataN = 10e2;

void randomData(char a[]) {
	srand((int)time(0));
	for(int i = 0; i<dataN; i++) {
		a[i] = char(rand()%26 + 'a');
	}
}