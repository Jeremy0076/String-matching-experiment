#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#include<unistd.h>
#include <sys/resource.h>
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

long getMemoryUsage() {
  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage))
    return usage.ru_maxrss; // bytes
  else
    return 0;
}

struct Trie {
	vector<Trie*> child;
	bool isWord = false;
	// Trie树每个节点增加坐标list
	vector<int> posList;

	Trie(): child(26), isWord(false) {}

	void insert(char s[], ll pos, ll lens) {
		Trie* t = this;
		for(int i = pos; i<lens; i++) {
			char c = s[i];
			if(t->child[c - 'a'] == NULL) 
				t->child[c - 'a'] = new Trie();
			t = t->child[c - 'a'];
			// 按匹配字符坐标顺序从小到大添加进posList
			t->posList.push_back(i);
		}
		t->isWord = true;
	}

	int search(char s[], ll pos, ll lens) {
		Trie* t = this;
		for(int i = pos; i<lens; i++) {
			// -1 为无匹配串
			char c = s[i];
			if(t->child[c - 'a'] == NULL)
				return 0;
			t = t->child[c - 'a'];
		}
		// 最后匹配节点的posliat数就是匹配的重叠串数, 重叠串:s = "ababa", p = "aba" 
		ll trueStringNum = 0, dupStringNum = t->posList.size();
		// 利用最后一个tire节点的posList和lenp(函数内=lens)去重, posList已经有序
		ll lastpos = -1;
		for(int i = 0; i<dupStringNum; i++) {
			if(lastpos >= t->posList[i]-lens+1) {
				continue;
			}
			lastpos = t->posList[i];
			Node *tmp = new Node(t->posList[i]-lens+1, t->posList[i]+1);
			matchRes.posList.push_back(tmp);
		}
		usedMem = maxll(usedMem, getMemoryUsage());
		trueStringNum = matchRes.posList.size();
		return trueStringNum;
	}
}tire;

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

void calculateTime(timeval& tStart, timeval& tFinish) {
	// 转ms
	usedTime = (tFinish.tv_sec-tStart.tv_sec)*1000000 + (tFinish.tv_usec-tStart.tv_usec);
	printf("执行耗时: %.10lfms\n", usedTime);
}

void calculateMem() {
	printf("内存占用: %.2lfMB\n", (double)usedMem/(1024*1024));
}

void trieMatch(char p[]) {
	timeval tStart, tFinish;
	gettimeofday(&tStart, NULL);
	matchRes.matchStringNum = tire.search(p, 0, strlen(p));
	gettimeofday(&tFinish, NULL);
	outputMatchRes();
	calculateTime(tStart, tFinish);
	usedMem = maxll(usedMem, getMemoryUsage());
	calculateMem();
}

void randomData(char a[]) {
	srand((int)time(0));
	for(int i = 0; i<dataN; i++) {
		a[i] = char(rand()%26 + 'a');
	}
}

int main() {
	randomData(s); 
	ll lens = strlen(s);
	for(int i = 0; i<lens; i++) 
		tire.insert(s, i, lens);
	while(scanf("%s", p) != EOF) {
		usedMem = maxll(usedMem, getMemoryUsage());
		matchRes.matchStringNum = 0;
		matchRes.posList = vector<Node*>();
		trieMatch(p);
	}
	
	return 0;
}
