#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct edge {
	int from, to, cap, rev;
};


class MaxFlow {
private:
	int source;
	int sink;
	int flow;
	int head, tail; //队列首尾
	int *q;  // 用于计算最大流的队列,用于BFS
	int *pprev;  // 指向前一个结点，用于BFS
	int *used;  // 用于BFS
	vector<vector<edge> > adj;
	int augmenting_path();  //寻找增光路径，用于BFS
	bool *check;  //用于source和sink

public:
	//默认构造函数
	MaxFlow();
	//构造函数
	MaxFlow(int n);
	//析构函数
	~MaxFlow();
	// 返回source
	int s() { return source; }
	// 返回sink
	int t() { return sink; }
	// 获取流
	int get_flow() { return flow; }
	// 添加边
	void add_edge(int from, int to, int cap, int rcap = 0);
	// 更新边
	void update_edge(int from, int to, int cap, int rcap = 0);
	//用于计算最大流
	int max_flow();
	// 用于存放source,sink点的容器
	vector<int> source_component();
	vector<int> sink_component();
	//显示计算过程
	void display();
};

#endif
