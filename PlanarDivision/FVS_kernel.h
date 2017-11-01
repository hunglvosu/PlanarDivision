#ifndef FVS_KERNEL_H
#define FVS_KERNEL_H

#include "stdafx.h"


namespace fvs_kernel {

	struct pair_compare{
		bool operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2) const {
		if (p1.first == p2.first)
			return p1.second < p2.second;
		return p1.first < p2.first;
	}
	};


	struct pair_hash {
		size_t operator()(const std::pair<int, int>& p) const {
			std::hash<int> h;
			return h(p.first) ^ h(p.second);
		}
	};


	// define edge element in the adjacency list
	struct edge {
		std::pair<int, int> endpts; // assume p1 <= p2
		int multi; // multiplicity of the edge
		edge(int a, int b, int m = 1) {
			int p1 = std::min(a, b);
			int p2 = std::max(a, b);
			endpts = std::make_pair(p1, p2);
			multi = m;
		}
		edge(const edge& e) {
			endpts = std::make_pair(e.endpts.first, e.endpts.second);
			multi = e.multi;
		}
	};

	struct vertex {
		std::list<edge> edges;
		int id;
		vertex(int i, std::list<edge>& l) {
			id = i;
			for (auto& e : l)
				edges.push_back(edge(e));
		}
		vertex(const vertex& v) {
			id = v.id;
			for (auto& e : v.edges)
				edges.push_back(edge(e));
		}
	};

	struct rDiv {
		std::vector<int> boundary;
		std::list<std::list<vertex>> graphs;
		std::vector<int> opt;
	};



	// map from a pair of vertex id to a pair of iterators of edges
	//typedef unordered_map<pair<int, int>, pair<list<edge>::iterator, list<edge>::iterator>, pair_hash > adjacency;
	typedef std::map< std::pair<int, int>, std::pair<std::list<edge>::iterator, std::list<edge>::iterator>, pair_compare > adjacency;



	// map from a pair of vertex id to a set of ids
	//typedef unordered_map<pair<int, int>, unordered_set<int>, pair_hash > neighbor;
	typedef std::map< std::pair<int, int>, std::unordered_set<int>, pair_compare > neighbor;


	// map from vertex id to its iterator in graph
	typedef std::unordered_map<int, std::list<vertex>::iterator> indices;



	struct config9 {
		// configuration for Rule 9
		// used for recover origianl solution from kernel solution
		int u1, u2, u3, u4, w1, w2, y, deg_u1;
		config9(int a, int b, int c, int d, int e, int f, int g, int deg) {
			u1 = a;
			u2 = b;
			u3 = c;
			u4 = d;
			w1 = e;
			w2 = f;
			y = g;
			deg_u1 = deg;
		}
	};

	class FVS_kernel {

	private:
		// the vertices belonging to the optimal solution
		std::unordered_set<int> opt;

		// map from vertex id to its iterator in graph K
		indices ids;

		// The value is a pair of two iterators for the edges in 
		// the adjacency lists of the corresponding endpoints. 
		// The first iterator corresponds the first endpoint.
		adjacency D1;

		// the value is a set of common neighbors of the edge's endpoints such that each vertex in the set has at least one more neighbor different from the two endpoints
		neighbor D2;

		// the set stores all pairs whose value in D2 has size greater than 2
		//unordered_set<pair<int, int>, pair_hash > q3;
		std::set<std::pair<int, int>, pair_compare > q3;

		// the set stores vertices with at most four neighbors
		std::unordered_set<int> qs;

		// FVS kernel graph
		std::list<vertex> G;

		// original graph
		std::list<vertex> H;

		// The list stores the subgraph used in Rule 9
		// Given a solution for kernel, this will be used to 
		// find the solution of original graph 
		std::list<config9> L9;


		int get_degree(int v);
		std::vector<int> get_neighbors(int v);
		bool simple(int a, int b);
		bool adjacent(int a, int b);
		void add_D2(int x, int y, int z);
		void remove_D2(int x, int y, int z);
		void remove_vertex(int v);
		void add_edge(int a, int b);
		void contract_edge(int a, int b);
		std::unordered_set<int> check15(int u);
		std::unordered_set<int> R6(int w, int v);
		void add_S_to_q(int a, int b);
		std::unordered_set<int> R7(int u);
		bool equals(std::vector<int>nv, int a, int b, int c);
		std::unordered_set<int> applyR8(int v1, int v2, int v3, int u1, int u2, int w1, int w2);
		int the_other(int x, std::pair<int, int>& p);
		std::unordered_set<int> R8(int x);
		std::unordered_set<int> applyR9(int u1, int u2, int u3, int u4, int w1, int w2);
		std::unordered_set<int> R9(int x);
		bool at_most_one_edge(int x, std::set<int> s);
		std::unordered_set<int> applyR101(int v1, int v2, int u, int w1, int w2);
		std::unordered_set<int> R101(int x);
		std::unordered_set<int> checkR102(int u1, int u2, int u3, int w1, int w2);
		std::unordered_set<int> R102(int x);
		std::unordered_set<int> applyR11(int u, int v, int v1, int v2, int v3, int v4, int v5, int v6, int w1, int w2);
		std::unordered_set<int> R11(int x);





	public:

		//FVS_kernel(string file);
		FVS_kernel(std::list< std::vector<int> >& edges);
		std::list<vertex> compute_kernel();
		std::list<vertex> get_kernel() { return G; }
		std::list<vertex> get_original() { return H; }
		std::unordered_set<int> get_opt() { return opt; }
		void recover_solution();
		void add_solution(std::vector<int> newset);


	};

	// generate edge list from file
	std::list< std::vector<int> > generate_edges(std::string file);

	// compute FVS for a graph with an r-division
	std::vector<int> compute_FVS(std::vector<int> Bound, std::list<std::list<vertex> >& Regions, std::list<vertex>& G);

}
#endif
