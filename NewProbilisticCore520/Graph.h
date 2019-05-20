
#include <vector>
#include <map>
using namespace std;

struct Edge {
	int to, next;
	double p;
};


class Graph
{
public:
	Graph();
	virtual ~Graph();
	void init(int n);
	void add_Edge(int tx, int ty, double p);
	int get_N();
	int get_M();
	int get_Deg(int i);
	bool isNeighbor(int x, int y);
	double get_p(int x, int id);
	int* get_Neighbor(int x);
	int* get_VertexList();
	void delete_Vert(int x);
	double get_min_p();

protected:

private:
	int n, m, tot;
	bool f_vid = false;
	double min_p;
	vector<Edge>  edg;
	map<int, int> first;
	map<int, int> deg;
	map<int, int> Vid;
	map<int, bool> fx;
};

