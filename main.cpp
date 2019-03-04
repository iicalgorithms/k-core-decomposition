#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<fstream>
#include<algorithm>
using namespace std;

int n,m,cnt,md;
int deg[5010];
struct Edge{
    int to,next;
}edg[100010];
int first[5010],color[5010],bin[5010],vert[5010], pos[5010];

void add_edge(int x,int y){
    edg[++cnt].to = y;
    edg[cnt].next = first[x];
    first[x] = cnt;

    edg[++cnt].to = x;
    edg[cnt].next = first[y];
    first[y] = cnt;
}

int main(){
    n = 15; m = 23, md = 0;
    for(int i=1; i<=n; i++) first[i] = -1;
    char buffer[256];
    ifstream in("text.txt");
    if(!in.is_open()){
        printf("Error opening file");
        return 0;
    }
    while(!in.eof()){
        in.getline(buffer,100);
        //printf("%s\n",buffer);
        int len = strlen(buffer);
        int x1 = 0,x2 = 0, k = 0;

        for(k=0; k<len; k++){
            if(buffer[k] == ' ') break;
            x1 = x1 * 10 + (int)(buffer[k] - '0');
        }
        k++;
        for(; k<len; k++){
            x2 = x2 * 10 + (int)(buffer[k] - '0');
        }
        //printf("%d %d\n",x1,x2);
        if(x1 == 0 && x2 ==0) break;
        x1++; x2++;
        add_edge(x1,x2);
        deg[x1]++; deg[x2]++;
    }
    in.close();
    for(int v=1; v<=n; v++) md = max(md, deg[v]);
    for(int d=0; d<=md; d++) bin[d] = 0;
    for(int v=1; v<=n; v++) bin[deg[v]]++;
    int start = 1;
    for(int d=0; d<=md; d++){
        int num = bin[d];
        bin[d] = start;
        start += num;
    }
    for(int v=1; v<=n; v++){
        pos[v] = bin[deg[v]];
        vert[pos[v]] = v;
        bin[deg[v]]++;
    }
    for(int d=md; d>=1; d--) bin[d] = bin[d-1];
    bin[0] = 1;
    for(int i=1; i<=n; i++){
        int v = vert[i];
        int k = first[v];
        while(k != -1){
            int u = edg[k].to;
            if(deg[u] > deg[v]){
                int du = deg[u], pu = pos[u];
                int pw = bin[du], w = vert[pw];
                if(u != w){
                    pos[u] = pw; vert[pu] = w;
                    pos[w] = pu; vert[pw] = u;
                }
                bin[du]++;
                deg[u]--;
            }
            k = edg[k].next;
        }
    }
    for(int i=1; i<=n; i++) printf("%d\n",deg[i]);
    return 0;
}
