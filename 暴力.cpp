#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#include<unistd.h>
#include <sys/resource.h>
#include<ctime>
#include<cstdlib>
#define ll long long
using namespace std;

const int N = 2010000;
const int dataN = 1e6; // 随机串s的长度
char s[N];
char p[N];
double usedTime;
ll usedMem;

struct Node {
	int start = 0;
	int end = 0;
	Node(): start(0), end(0) {}
	Node(int a, int b): start(a), end(b) {}
};

struct MatchResult {
	ll matchStringNum = 0;
	char *pattern = NULL;
	vector<Node*> posList;
}matchRes;

ll maxll(ll a, ll b) {
	if (a > b) 
		return a;
	return b;
}

void calculateTime(timeval& tStart, timeval& tFinish) {
	// 转ms
	usedTime = (tFinish.tv_sec-tStart.tv_sec)*1000000 + (tFinish.tv_usec-tStart.tv_usec);
	printf("执行耗时: %.10lfms\n", usedTime);
}

long getMemoryUsage() {
  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage))
    return usage.ru_maxrss; // bytes
  else
    return 0;
}

void calculateMem() {
	printf("内存占用: %.2lfMB\n", (double)usedMem/(1024*1024));
}

void outputMatchRes() {
	printf("匹配串: %s 匹配数量: %lld\n", matchRes.pattern, matchRes.matchStringNum);
	int posListSize = matchRes.posList.size();
	if (posListSize == 0) {
		printf("无匹配串\n");
		return;
	}
	printf("匹配坐标:\n");
	for(int i = 0; i<posListSize; i++) {
		printf("启始:%d 终止:%d\n", matchRes.posList[i]->start, matchRes.posList[i]->end);
	}
}

void BF(char s[], char p[], ll lens, ll lenp) {
	usedMem = maxll(usedMem, getMemoryUsage());
	for(int i = 0; i<lens; i++) {
		int j = 0;
		int start = i;
		while(i<lens && j<lenp && s[i] == p[j]) 
			i++, j++;
		if (j == lenp) {
			Node* tmp = new Node(start, start + lenp);
			matchRes.posList.push_back(tmp);
			matchRes.matchStringNum++;
			//模拟vscode cmd+f, 不存在重叠
			i = start+lenp;
		}
		i = start;
	}
	usedMem = maxll(usedMem, getMemoryUsage());

}

void randomData(char a[]) {
	srand((int)time(0));
	for(int i = 0; i<dataN; i++) {
		a[i] = char(rand()%26 + 'a');
	}
}

int main() {
	randomData(s); 
	timeval tStart, tFinish;
	
	while(scanf("%s", p) != EOF) {
	//暴力匹配
		usedMem = maxll(usedMem, getMemoryUsage());
		matchRes.matchStringNum = 0;
		matchRes.posList = vector<Node*>();
		ll lens = strlen(s), lenp = strlen(p);
		gettimeofday(&tStart, NULL);
		BF(s, p, lens, lenp);
		gettimeofday(&tFinish, NULL);
		outputMatchRes();
		calculateTime(tStart, tFinish);
		usedMem = maxll(usedMem, getMemoryUsage());
		calculateMem();
	}
	
	return 0;
}

// abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab
// ab
