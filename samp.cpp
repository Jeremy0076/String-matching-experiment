#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#define ll long long
using namespace std;

const int N = 2010000;
const int dataN = 1e6; // 随机串s的长度
char s[N];
char p[N];  
int t[N], A[N];
double usedTime;
ll usedMem;

int len[N], link[N]; // len: maxlen数组, link: 后缀连接数组
int to[N][26]; // to: trans函数 状态转移函数
int siz[N]; // 状态i的endpos集合数

struct State {
	vector<int> posList; // 记录每个状态的endpos
}st[N];

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

struct SAM {
	int size, last; // size: 节点个数, last: 上一个更新的节点位置
	
	SAM() : size(1), last(1) {}

	// 为当前的SAM增加一个字符c
	// cur = z, p = 遍历所有情况, cl = y, q = x, last = pre_v
	void extend(int c, int pos) {
		int p, cur = ++size;
		len[cur] = len[last] + 1;
		siz[cur] = 1;
		st[cur].posList.push_back(pos);
		// 情况1
		for (p = last; p && !to[p][c]; p = link[p])
			to[p][c] = cur;
		if (!p) link[cur] = 1;
		else {
			int q = to[p][c];
			// 情况 2 - A类
			if (len[q] == len[p] + 1) {
				link[cur] = q;
				int fa = link[cur];
				// 后缀路径上依次添加endpos
				while(fa != 1) {
					st[fa].posList.push_back(pos);
					fa = link[fa];
				}
			} 
			else {
				// 情况 2 - B类
				int cl = ++size;
				len[cl] = len[p] + 1;
				memcpy(to[cl], to[q], sizeof(to[cl]));
				link[cl] = link[q];
				st[cl].posList = st[q].posList;
				while (p && to[p][c] == q) {
					to[p][c] = cl;
					p = link[p];
				}
				link[cur] = link[q] = cl;
			}
		}
		last = cur;
	}

	int search(char p[]) {
		int pos = 1;
		int lenp = strlen(p);
		int i = 0;
		for(i = 0; i<lenp; i++) {
			char c = p[i];
			if (to[pos][c - 'a'] == 0) break;
			else pos = to[pos][c - 'a'];
		}
		if(i < lenp) return 0;
		// 去重
		ll dupStringNum = st[pos].posList.size(), trueStringNum = 0;
		int lastpos = -1;
		for(i = 0; i<dupStringNum; i++) {
			if(lastpos >= st[pos].posList[i]-lenp+1) {
				continue;
			}
			lastpos = st[pos].posList[i]+1;
			Node *tmp = new Node(st[pos].posList[i]-lenp+1, st[pos].posList[i]+1);
			matchRes.posList.push_back(tmp);
			trueStringNum++;
		}
		usedMem = maxll(usedMem, getMemoryUsage());
		return trueStringNum;
	}

}sam;

void calculateTime(timeval& tStart, timeval& tFinish) {
	// 转ms
	usedTime = (tFinish.tv_sec-tStart.tv_sec)*1000000 + (tFinish.tv_usec-tStart.tv_usec);
	printf("执行耗时: %.10lfms\n", usedTime);
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

void samMatch(char s[], char p[]) {
	timeval tStart, tFinish;
	usedMem = maxll(usedMem, getMemoryUsage());
	gettimeofday(&tStart, NULL);
	matchRes.matchStringNum = sam.search(p);
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
	// part 1 构建后缀自动机sam
	for(int i = lens; i>=1; i--) s[i] = s[i-1]; s[0] = 0;
	for(int i = 1; i<=lens; i++) sam.extend(s[i] - 'a', i-1);

	// part 2 按len排序, 反向拓扑模拟parent树的DFS（后序遍历）
	//1.计数排序 按len从小到大放入A[] 值为状态i
	for(int i = 1; i<=sam.size; i++) t[len[i]]++;
	for(int i = 1; i<=sam.size; i++) t[i]+=t[i-1];
	for(int i = 1; i<=sam.size; i++) A[t[len[i]]--]=i;
	//2.反向拓扑模拟parent树的DFS
	for(int i = sam.size; i>=1; i--) {
		int now = A[i];
		siz[link[now]]+=siz[now];
	}
	
	while(scanf("%s", p)) {
		matchRes.matchStringNum = 0;
		matchRes.posList = vector<Node*>();
		usedMem = maxll(usedMem, getMemoryUsage());
		samMatch(s, p);
	}
	
	return 0;
}
