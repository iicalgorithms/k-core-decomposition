// UncertainCore_NewBasic.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "Graph.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <fstream>
#include <ctime>
#include <algorithm>
using namespace std;
#define MAXN 0x7fffffff;
#define random(x) (rand() % x)

int* Calc_Deg(Graph* G, double eta) {
	int n = G->get_N();
	int *P_deg = new int[n + 5];
	int *VertexList = G->get_VertexList();
	//for (int i = 1; i <= n; i++) printf("%d ", VertexList[i]); printf("\n");

	for (int uu = 1; uu <= n; uu++) {
		int tx = VertexList[uu];
		int mx = G->get_Deg(tx);
		double **PX = new double*[2];
		for (int i = 0; i < 2; i++) PX[i] = new double[mx + 10];
		PX[0][0] = 1;
		for (int i = 1; i <= mx; i++) {
			double Ph = G->get_p(tx, i);
			PX[i % 2][0] = (1.0 - Ph) * PX[(i - 1) % 2][0];
			for (int j = 1; j <= i - 1; j++) {
				PX[i % 2][j] = Ph * PX[(i - 1) % 2][j - 1] + (1.0 - Ph) * PX[(i - 1) % 2][j];
			}
			PX[i % 2][i] = Ph * PX[(i - 1) % 2][i - 1];
		}
		bool flag = true;
		for (int i = mx; i >= 0; i--) {
			if (PX[mx % 2][i] >= eta) {
				//	printf("%d %d %.3f %.3f\n", tx, i, PX[mx % 2][i],P);
				P_deg[uu] = i;
				flag = false;
				break;
			}
		}
		delete[] PX;
		if (flag) P_deg[uu] = 0;
	}
	delete[] VertexList;
	return P_deg;
}

int Calc_u_Deg(Graph* G, double eta, int ti, int tx) {
	int ans = 0;
	int *NeighborList = G->get_Neighbor(tx);
	int mx = G->get_Deg(tx);
	double **PX = new double*[2];
	for (int i = 0; i < 2; i++) PX[i] = new double[mx + 5];
	PX[0][0] = 1;
	for (int i = 1; i <= mx; i++) {
		double Ph = G->get_p(tx, i);
		PX[i % 2][0] = (1.0 - Ph) * PX[(i - 1) % 2][0];
		for (int j = 1; j <= i - 1; j++) {
			PX[i % 2][j] = Ph * PX[(i - 1) % 2][j - 1] + (1.0 - Ph) * PX[(i - 1) % 2][j];
		}
		PX[i % 2][i] = Ph * PX[(i - 1) % 2][i - 1];
	}
	for (int i = mx; i >= 0; i--) {
		if (PX[mx % 2][i] >= eta) {
			ans = i;
			break;
		}
	}
	delete[] PX;
	delete[] NeighborList;
	return ans;
}

int* get_K_Core(Graph* G, double eta) {
	int n = G->get_N();
	int *P_deg = Calc_Deg(G, eta);
	ofstream out("P_deg.txt");
	for (int i = 1; i <= n; i++) {
		out << P_deg[i] << endl;
	}
	out.close();
	int *core = new int[n + 10];
	int *VertexList = G->get_VertexList();
	vector<set<int>> D;
	for (int i = 0; i <= n; i++) {
		set<int> s;
		D.push_back(s);
	}
	for (int i = 1; i <= n; i++) {
		D[P_deg[i]].insert(i);
	}

	for (int k = 0; k <= n; k++) {
		while (!D[k].empty()) {
			set<int>::iterator iter = D[k].begin();
			int v = *iter;
			D[k].erase(v);
			core[v] = k;
			int *NeighborList = G->get_Neighbor(VertexList[v]);
			int de = G->get_Deg(VertexList[v]);
			G->delete_Vert(VertexList[v]);
			for (int j = 1; j <= de; j++) {
				int u = NeighborList[j];
				if (P_deg[u] > k) {
					int new_deg = Calc_u_Deg(G, eta, u, VertexList[u]);
					D[P_deg[u]].erase(u);   D[new_deg].insert(u);
					P_deg[u] = new_deg;
				}
			}
			delete[] NeighborList;
		}
	}
	delete[] VertexList;
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
	bool is_oneLine = true;
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
		if (is_oneLine) {
			/*for (k; k < len; k++) {
				if (buffer[k] == '.') break;
				P = P * 10.0 + 1.0 * (int)(buffer[k] - '0');
			}
			k++;
			px = 0.1;
			for (k; k < len; k++) {
				P = P + px * (int)(buffer[k] - '0');
				px *= 0.1;
			}*/
			G->init(x1);
			is_oneLine = false;
		}
		else {
			for (; k < len; k++) {
				x2 = x2 * 10 + (int)(buffer[k] - '0');
			}
			if (x1 == 0 && x2 == 0) break;
			x1++; x2++;
			double px = 0.1 * (random(10) + 1);
			//printf("%d %d %.2f \n", x1, x2, px);
			G->add_Edge(x1, x2, px);
			/*for (; k < len; k++) {
				if (buffer[k] == '	') break;
				x2 = x2 * 10 + (int)(buffer[k] - '0');
			}
			k++;
			for (; k < len; k++) {
				if (buffer[k] == '.') break;
				px = px * 10.0 + 1.0 * (int)(buffer[k] - '0');
			}
			double td = 0.1;
			k++;
			for (; k < len; k++) {
				px = px + td * (int)(buffer[k] - '0');
				td *= 0.1;
			}
			if (x1 == 0 && x2 == 0) break;
			x1++; x2++;
			G->add_Edge(x1, x2, px);*/
		}
	}
	in.close();
	printf("Please print eta : ");
	cin >> P;

	int *ans = get_K_Core(G, P);

	int n = G->get_N();
	ofstream out("Basic.txt");
	for (int i = 1; i <= n; i++) {
		out << ans[i] << endl;
	}
	out.close();

	return 0;
}

