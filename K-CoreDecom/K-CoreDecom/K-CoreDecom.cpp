// K-CoreDecom.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Graph.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <fstream>
#include <ctime>
#include <algorithm>
using namespace std;
#define MAXN 0x7fffffff;

int computeIndex(int *est, int n, int u, int k) {
	int *cnt = new int[k + 10];
	for (int i = 1; i <= k; i++) cnt[i] = 0;
	//   printf("u = %d, k = %d\n", u, k);
	for (int i = 1; i <= n; i++) {
		int j = min(k, est[i]);
		cnt[j]++;
	}
	for (int i = k; i >= 2; i--) {
		//     printf("cnt[%d] = %d\n",i,cnt[i]);
		cnt[i - 1] += cnt[i];
	}
	int i = k;
	while (i > 1 && cnt[i] < i) i--;
	return i;
}

int* get_K_Core(Graph *G) {
	int n = G->get_N();
	int m = G->get_M();
	bool *changed = new bool[n + 10];
	int *core = new int[n + 10];

	vector<int> Tchanged;
	vector<double> ETime;
	for (int i = 1; i <= n; i++) core[i] = G->get_Deg(i);
	printf("%d %d\n",n, core[12]);
	int tot = 0;
	clock_t startTime, endTime;

	while (true) {
		startTime = clock();
		for (int i = 1; i <= n; i++) changed[i] = false;

		for (int i = 1; i <= n; i++) {
			int *NeighborList = G->get_Neighbor(i);
			int de = G->get_Deg(i);
			int *NeighbotCore = new int[de + 5];
			for (int j = 1; j <= de; j++) {
				NeighbotCore[j] = core[NeighborList[j]];
			}
			int t = computeIndex(NeighbotCore, de, i, core[i]);
			if (t < core[i]) {
				//printf("%d %d %d %d\n",tot,i,t,core[i]);
				core[i] = t;
				changed[i] = true;
			}
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
		printf("This %d round, Not Changed Nodes = %d, Run Times = %.6f.   \n", tot, Tchanged[tot - 1], nowTime);
		if (Tchanged[tot - 1] == n) break;
	}
	return core;
}

int main()
{
	char buffer[256];
	Graph *G = new Graph;
	ifstream in("p2p-Gnutella31.txt");
	//ifstream in("text.txt");
	if (!in.is_open()) {
		printf("Error opening file");
		return 0;
	}
	while (!in.eof()) {
		in.getline(buffer, 100);
		int len = strlen(buffer);
		int x1 = 0, x2 = 0, k = 0;

		for (k; k < len; k++) {
			if (buffer[k] == '	') break;
			x1 = x1 * 10 + (int)(buffer[k] - '0');
		}
		k++;
		if (k >= len) {
			G->init(x1);
			//printf("%d\n",x1);
		}
		else {
			for (; k < len; k++) {
				x2 = x2 * 10 + (int)(buffer[k] - '0');
			}
			if (x1 == 0 && x2 == 0) break;
			//  printf("%d %d \n",x1,x2);
			x1++; x2++;
			G->add_Edge(x1, x2);
		}
	}
	in.close();
	int *ans;

	ans = get_K_Core(G);

	ofstream outf("ans.txt");
	if (!outf.is_open()) {
		printf("Error opening file");
		return 0;
	}
	int n = G->get_N();
	outf << n << endl;
	for (int i = 1; i <= n; i++) {
		outf << ans[i] << endl;
	}

	return 0;
}


