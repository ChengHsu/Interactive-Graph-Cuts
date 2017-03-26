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
	int head, tail; //������β
	int *q;  // ���ڼ���������Ķ���,����BFS
	int *pprev;  // ָ��ǰһ����㣬����BFS
	int *used;  // ����BFS
	vector<vector<edge> > adj;
	int augmenting_path();  //Ѱ������·��������BFS
	bool *check;  //����source��sink

public:
	//Ĭ�Ϲ��캯��
	MaxFlow();
	//���캯��
	MaxFlow(int n);
	//��������
	~MaxFlow();
	// ����source
	int s() { return source; }
	// ����sink
	int t() { return sink; }
	// ��ȡ��
	int get_flow() { return flow; }
	// ��ӱ�
	void add_edge(int from, int to, int cap, int rcap = 0);
	// ���±�
	void update_edge(int from, int to, int cap, int rcap = 0);
	//���ڼ��������
	int max_flow();
	// ���ڴ��source,sink�������
	vector<int> source_component();
	vector<int> sink_component();
	//��ʾ�������
	void display();
};

#endif
