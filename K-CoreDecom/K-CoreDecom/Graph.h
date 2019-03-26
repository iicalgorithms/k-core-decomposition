#pragma once
#include <vector>
using namespace std;

struct Edge {
	int to, next;
};


class Graph
{
public:
	Graph();
	virtual ~Graph();
	void init(int n);
	void add_Edge(int x, int y);
	int get_N();
	int get_M();
	int get_Deg(int i);
	bool isNeighbor(int x, int y);

protected:

private:
	int n, m, tot;
	vector<Edge>  edg;
	vector<int> first;
	vector<int> deg;
};
