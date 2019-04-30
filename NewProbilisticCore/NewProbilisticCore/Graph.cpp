#include "pch.h"
#include "Graph.h"
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

Graph::Graph()
{
	//ctor
}

Graph::~Graph()
{
	//dtor
}


void Graph::init(int tn) {
	n = tn;
	m = tot = 0;
	//for (int i = 0; i <= n; i++) first.push_back(-1);
	//printf("%d %d\n",tn,n);
	//for (int i = 0; i <= n; i++) deg.push_back(0);
}

void Graph::add_Edge(int tx, int ty, double p) {
	m++;
	if (first.count(tx) == 0) {
		first.insert(pair<int, int>(tx, -1));
		deg.insert(pair<int, int>(tx, 0));
		//fx.insert(pair<int, int>(tx, true));
	}
	if (first.count(ty) == 0) {
		deg.insert(pair<int, int>(ty, 0));
		first.insert(pair<int, int>(ty, -1));
		//fx.insert(pair<int, int>(ty, true));
	}
	Edge e1;
	e1.to = ty;
	e1.next = first[tx];
	e1.p = p;
	edg.push_back(e1);
	first[tx] = tot++;

	e1.to = tx;
	e1.next = first[ty];
	e1.p = p;
	edg.push_back(e1);
	first[ty] = tot++;

	deg[tx]++; deg[ty]++;
}

int Graph::get_N() {
	return n;
}

int Graph::get_M() {
	return m;
}

int Graph::get_Deg(int x) {
	//int ans = 0;
	//int k = first[x];
	//while (k != -1) {
	//	if (fx[edg[k].to]) ans++;
	//	k = edg[k].next;
	//}
	//return ans;
	return deg[x];
}

bool Graph::isNeighbor(int x, int y) {
	int k = first[x];
	while (k != -1) {
		if (y == edg[k].to) return true;
		k = edg[k].next;
	}
	return false;
}

double Graph::get_p(int x, int id) {
	int k = first[x];
	while (k != -1) {
		//if(fx[edg[k].to]) 
			id--;
		if (id == 0) return edg[k].p;
		k = edg[k].next;
	}
	return 0.0;
}

int* Graph::get_Neighbor(int x) {
	if (f_vid == false) {
		f_vid = true;
		map<int, int>::reverse_iterator it;
		int i = n;
		for (it = first.rbegin(); it != first.rend(); it++) {
			Vid.insert(pair<int, int>(it->first, i--));
		}
	}
	int *NeighborList = new int[deg[x] + 5];
	int i = 1;
	int k = first[x];
	while (k != -1) {
		//if(fx[edg[k].to]) 
			NeighborList[i++] = Vid[edg[k].to];
		k = edg[k].next;
	}
	return NeighborList;
}

int* Graph::get_VertexList() {
	int *VertexList = new int[n + 10];
	map<int, int>::reverse_iterator it;
	int i = n;
	for (it = first.rbegin(); it != first.rend(); it++) {
		VertexList[i--] = it->first;
	}
	return VertexList;
}

//void Graph::delete_Vert(int x) {
//	fx[x] = false;
//}

//void Graph::Re_Vert(int x) {
//	fx[x] = true;
//}
