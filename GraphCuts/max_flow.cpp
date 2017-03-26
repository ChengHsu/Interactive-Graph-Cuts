#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "max_flow.h"

const int INF = 999999999;

// 默认构造函数
MaxFlow::MaxFlow() : source(0), sink(0), flow(0), adj() {
	q = new int[0]();
	pprev = new int[0]();
	used = new int[0]();
	check = new bool[0]();
}

//构造函数，n表示有向图结点数(source,sink除外）
MaxFlow::MaxFlow(int n) : source(n), sink(n + 1), flow(0), adj() {
	q = new int[n + 5]();
	pprev = new int[n + 5]();
	used = new int[n + 5]();
	check = new bool[n + 5]();
	for (int i = 0; i < n + 5; ++i) {
		adj.push_back(vector<edge>());
	}
}

MaxFlow::~MaxFlow() {
	delete[] pprev;
	delete[] used;
	delete[] q;
	delete[] check;
}

// 添加有向边
void MaxFlow::add_edge(int from, int to, int cap, int rcap) {
	edge e1, e2;
	e1.from = from; e1.to = to;
	e2.from = to; e2.to = from;
	e1.cap = cap; e2.cap = rcap;
	e1.rev = adj[to].size();
	e2.rev = adj[from].size();
	adj[from].push_back(e1);
	adj[to].push_back(e2);
}

//更新边
void MaxFlow::update_edge(int from, int to, int cap, int rcap) {
	for (int i = 0; i < adj[from].size(); ++i) {
		if (adj[from][i].to == to) {
			adj[from][i].cap += cap;
			adj[to][adj[from][i].rev].cap += rcap;
			break;
		}
	}
}


//增广路径
int MaxFlow::augmenting_path() {
	for (int i = 0; i <= sink; ++i) {
		pprev[i] = -1;
	}
	head = 0;
	tail = 1;
	q[head] = source;
	while (head < tail && pprev[sink] == -1) {
		int next = q[head++];
		for (int i = 0; i < adj[next].size(); ++i) {
			if (pprev[adj[next][i].to] == -1 && adj[next][i].cap > 0 && adj[next][i].to != source) {
				pprev[adj[next][i].to] = next;
				used[adj[next][i].to] = i;
				q[tail++] = adj[next][i].to;
			}
		}
	}

	if (pprev[sink] == -1) {
		return 0;
	}
	else {
		int cur = sink;
		int f = INF;
		//寻找残余流量的最小值f
		while (cur != source) {
			int p = pprev[cur];
			f = min(f, adj[p][used[cur]].cap);
			cur = p;
		}
		// 将每条边的流量减去f，然后反向更新，将每条边反方向的流量加f
		// 反向更新是为继续寻找下一条增广路径提供了可能
		cur = sink;
		while (cur != source) {
			int p = pprev[cur];
			adj[p][used[cur]].cap -= f;
			adj[cur][adj[p][used[cur]].rev].cap += f;
			cur = p;
		}
		return f;
	}
}

// 每找到一条增广路径将当前最大流量+f,直到找不到增广路径为止，此时当前最大流量即为最大流量
int MaxFlow::max_flow() {
	while (true) {
		int f = augmenting_path();
		flow += f;
		if (f == 0) {
			break;
		}
	}
	return flow;
}

// source 
vector<int> MaxFlow::source_component() {
	vector<int> res;
	for (int i = 0; i <= sink; ++i) {
		check[i] = false;
	}
	for (int i = 0; i < tail; ++i) {
		check[q[i]] = true;
	}
	for (int i = 0; i <= sink; ++i) {
		if (check[i]) {
			res.push_back(i);
		}
	}
	return res;
}

// sink
vector<int> MaxFlow::sink_component() {
	vector<int> res;
	for (int i = 0; i <= sink; ++i) {
		check[i] = false;
	}
	for (int i = 0; i < tail; ++i) {
		check[q[i]] = true;
	}
	for (int i = 0; i <= sink; ++i) {
		if (!check[i]) {
			res.push_back(i);
		}
	}
	return res;
}

//  显示计算过程
void MaxFlow::display() {
	cout << "source :" << source << "  sink : " << sink << endl;
	cout << "current flow = " << flow << endl;
	for (int i = 0; i <= sink; ++i) {
		cout << " For " << i << endl;
		for (int j = 0; j < adj[i].size(); ++j) {
			cout << "\t" << adj[i][j].from << "->" << adj[i][j].to << ": " << adj[i][j].cap << endl;
		}
	}
}
