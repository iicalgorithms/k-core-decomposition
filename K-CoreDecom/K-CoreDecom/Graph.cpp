#include "pch.h"
#include "Graph.h"
#include <vector>
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
	for (int i = 0; i <= n; i++) first.push_back(-1);
	//printf("%d %d\n",tn,n);
	for (int i = 0; i <= n; i++) deg.push_back(0);
}

void Graph::add_Edge(int tx, int ty) {
	m++;
	Edge e1;
	e1.to = ty;
	e1.next = first[tx];
	edg.push_back(e1);
	first[tx] = tot++;

	e1.to = tx;
	e1.next = first[ty];
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

int Graph::get_Deg(int i) {
	return deg[i];
}

bool Graph::isNeighbor(int x, int y) {
	int k = first[x];
	while (k != -1) {
		if (y == edg[k].to) return true;
		k = edg[k].next;
	}
	return false;
}

