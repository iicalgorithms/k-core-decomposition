// ProbilisticCore_TwoStage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "Graph.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
#define MAXN 0x7fffffff;
#define random(x) (rand() % x)

struct node {
	int core;
	double p;
};

bool cmp(node a, node b) {
	if (a.p > b.p) return true;
	if (a.p == b.p && a.core > b.core) return true;
	return false;
}


int* Calc_Deg(Graph* G, double P, map<pair<int, int>, double>* Pr) {
	int n = G->get_N();
	int *P_deg = new int[n + 5];
	int *VertexList = G->get_VertexList();

	for (int uu = 1; uu <= n; uu++) {
		int tx = VertexList[uu];
		int *NeighborList = G->get_Neighbor(tx);
		int mx = G->get_Deg(tx);
		double **PX = new double*[2];
		for (int i = 0; i < 2; i++) PX[i] = new double[mx + 10];
		PX[0][0] = 1;
		for (int i = 1; i <= mx; i++) {
			pair<int, int> p1(uu, NeighborList[i]);
			double Ph = (*Pr)[p1];
			PX[i % 2][0] = (1.0 - Ph) * PX[(i - 1) % 2][0];
			for (int j = 1; j <= i - 1; j++) {
				PX[i % 2][j] = Ph * PX[(i - 1) % 2][j - 1] + (1.0 - Ph) * PX[(i - 1) % 2][j];
			}
			PX[i % 2][i] = Ph * PX[(i - 1) % 2][i - 1];
		}
		bool flag = true;
		for (int i = mx; i >= 0; i--) {
			if (PX[mx % 2][i] >= P) {
				P_deg[uu] = i;
				flag = false;
				break;
			}
		}
		delete[] PX;
		delete[] NeighborList;
		if (flag) P_deg[uu] = 0;
	}
	delete[] VertexList;
	return P_deg;
}

int computeIndex(node *est, int n, int k, int de) {
	sort(est + 1, est + 1 + n, cmp);
	int *cnt = new int[k + 10];
	for (int i = 1; i <= k; i++) cnt[i] = 0;
	for (int i = 1; i <= de; i++) {
		int j = min(k, est[i].core);
		cnt[j]++;
	}
	for (int i = k; i >= 2; i--) {
		cnt[i - 1] += cnt[i];
	}
	int i = k;
	while (i > 0 && cnt[i] < i) i--;
	delete[] cnt;
	return i;
}

int* get_K_Core(Graph* G, double P) {
	int n = G->get_N();
	int m = G->get_M();
	int *VertexList = G->get_VertexList();

	map<pair<int, int>, double> Pr;
	map<pair<int, int>, int> Count;
	for (int i = 1; i <= n; i++) {
		int tx = VertexList[i];
		int *VertNeighborList = G->get_Neighbor(tx);
		int de = G->get_Deg(tx);
		for (int j = 1; j <= de; j++) {
			pair<int, int> p1(i, VertNeighborList[j]);
			if (Pr.count(p1) == 0) {
				Pr.insert(pair<pair<int, int>, double>(p1, 0.0));
				Count.insert(pair<pair<int, int>, int>(p1, 0));
			}
		}
		delete[] VertNeighborList;
	}

	ofstream rout("Error_P.txt");
	int Round1 = (int)(ceil(8.0 * log(n) / G->get_min_p()));
	printf("%d\n", Round1);
	double Error_p;
	for (int Ro = 1; Ro <= Round1; Ro++) {
	//int Ro = 0;
	//while(true) {
		Error_p = 0.0;
		for (int i = 1; i <= n; i++) {
			int tx = VertexList[i];
			int *NeighborList = G->get_Neighbor(tx);
			int de = G->get_Deg(tx);
			for (int j = 1; j <= de; j++) {
				pair<int, int> p1(i, NeighborList[j]);
				double px = 0.1 * (random(10) + 1);
				double ex = G->get_p(tx, j);
				if (px <= ex) 	Count[p1]++;
				Pr[p1] = (1.0 * Count[p1]) / Ro;
				Error_p += fabs(Pr[p1] - ex);
			}
			delete[] NeighborList;
		}
		rout << Ro << '	' << Error_p << endl;
		//Ro++;
		//if (1.0 * Error_p / n < 0.03) break;
	}
	rout.close();

	printf("Stage1 Finish!\n");

	bool *changed = new bool[n + 10];
	for (int i = 1; i <= n; i++) changed[i] = false;
	int *deg = Calc_Deg(G, P, &Pr);
	int *core = new int[n + 10];
	for (int i = 1; i <= n; i++) core[i] = deg[i];
	vector<int> Tchanged;
	vector<double> ETime;
	int tot = 0;
	clock_t startTime, endTime;
	ofstream out("ans.txt");

	while (true) {
		printf("%d\n", tot);
		startTime = clock();
	    
		for (int i = 1; i <= n; i++) {
			int id = VertexList[i];
			int *NeighborList = G->get_Neighbor(id);
			int de = G->get_Deg(id);
			node *NeighborCore = new node[de + 5];
			for (int j = 1; j <= de; j++) {
				NeighborCore[j].core = core[NeighborList[j]];
				pair<int, int> p1(i, NeighborList[j]);
				NeighborCore[j].p = Pr[p1];
			}

			int t = computeIndex(NeighborCore, de, core[i], deg[i]);
			if (t < core[i]) {
				core[i] = t;
				changed[i] = true;
			}
			delete[] NeighborCore;
			delete[] NeighborList;
		}

		Tchanged.push_back(0);
		for (int i = 1; i <= n; i++) {
			if (changed[i] == false) Tchanged[tot]++;
			changed[i] = false;
		}
		tot++;
		endTime = clock();
		double nowTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		ETime.push_back(nowTime);
		out << "This" << tot << "round, Not Changed Nodes = " << Tchanged[tot - 1] << ", Run Times = " << nowTime << endl;
		//printf("This %d round, Not Changed Nodes = %d, Run Times = %.6f.   \n", tot, Tchanged[tot - 1], nowTime);
		if (Tchanged[tot - 1] == n) break;
	}
	out.close();
	delete[] VertexList;
	delete[] changed;
	delete[] deg;
	return core;
}

int main()
{
	srand((int)time(0));
	char buffer[256];
	Graph *G = new Graph;
	ifstream in("./Data/p2p-Gnutella31.txt");
	if (!in.is_open()) {
		printf("Error opening file");
		return 0;
	}
	bool is_oneline = true;
	double P = 0.0;
	while (!in.eof()) {
		in.getline(buffer, 100);
		int len = strlen(buffer);
		int x1 = 0, x2 = 0, k = 0;
		double px = 0.0;
		for (k; k < len; k++) {
			if (buffer[k] == '	') break;
			x1 = x1 * 10 + (int)(buffer[k] - '0');
		}
		k++;
		if (is_oneline) {
			G->init(x1);
			is_oneline = false;
		}
		else {
			for (; k < len; k++) {
				x2 = x2 * 10 + (int)(buffer[k] - '0');
			}
			if (x1 == 0 && x2 == 0) break;
			x1++; x2++;
			double px = 0.1 * (random(10) + 1);
			G->add_Edge(x1, x2, px);
		}
	}
	in.close();
	printf("Please print eta : ");
	cin >> P;

	int *ans = get_K_Core(G, P);

	int n = G->get_N();
	ofstream out("ProbilisticCore_TwoStage.txt");
	for (int i = 1; i <= n; i++) {
		out << ans[i] << endl;
	}
	out.close();

	return 0;
}

