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

int computeIndex(Graph *G, int *est, int u, int k) {
	int n = G->get_N();
	int *cnt = new int[k + 10];
	for (int i = 1; i <= k; i++) cnt[i] = 0;
	//   printf("u = %d, k = %d\n", u, k);
	for (int i = 1; i <= n; i++) {
		if (G->isNeighbor(u, i)) {
			int j = min(k, est[i]);
			cnt[j]++;
		}
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
	int **est = new int*[n + 10];

	vector<int> Tchanged;
	vector<double> ETime;
	for (int i = 1; i <= n; i++) core[i] = G->get_Deg(i);
	for (int i = 1; i <= n; i++) {
		est[i] = new int[n + 10];
		for (int j = 1; j <= n; j++) {
			if (G->isNeighbor(i, j)) {
				est[i][j] = MAXN;
			}
			else {
				est[i][j] = -1;
			}
		}
	}

	int tot = 0;
	clock_t startTime, endTime;
	while (true) {
		startTime = clock();
		for (int i = 1; i <= n; i++) changed[i] = false;

		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (est[i][j] != -1) {
					if (core[j] < est[i][j]) {
						est[i][j] = core[j];
						int t = computeIndex(G, est[i], i, core[i]);
						if (t < core[i]) {
							// printf("%d %d %d %d %d\n",tot,i,j,t,core[i]);
							core[i] = t;
							changed[i] = true;
						}
					}
				}
			}
		}

		Tchanged.push_back(0);
		for (int i = 1; i <= n; i++) {
			if (changed[i] == false) Tchanged[tot]++;
			changed[i] = false;
		}
		tot++;
		//  printf("%d %d\n",tot,Tchanged[tot-1]);
		endTime = clock();
		double nowTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		ETime.push_back(nowTime);
		if (Tchanged[tot - 1] == n || tot == 10) break;
	}
	printf("tot = %d\n", tot);
	for (int i = 0; i < tot; i++) {
		printf("Not Change = %d, Run Time = %.6f.  \n", Tchanged[i], ETime[i]);
	}
	return core;
}

int main()
{
	char buffer[256];
	Graph *G = new Graph;
	ifstream in("text.txt");
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
			//    printf("%d\n",x1);
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

	int *ans;

	ans = get_K_Core(G);

	int n = G->get_N();
	for (int i = 1; i <= n; i++) {
		printf("%d ", ans[i]);
	}
	printf("\n");

	return 0;
}


