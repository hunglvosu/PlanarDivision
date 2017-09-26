#pragma once
#include<iostream>
#include <vector>

typedef struct vertex vertex;
typedef struct arc arc;
//struct planargraph;


struct arc {
//	int sourceid;
//	int sourcename;

//	int sinkid;
//	int sinkname;
	vertex *source;
	vertex *sink;
	arc *nextarc;	// the next arc in the clock-wise rotational system
	arc *prevarc;	// the previous arc in the clock-wise rotational system 
	arc *rev;		// the rever of the arc
	int id;
	int index;
	bool mark;
};

struct vertex {
	int name;		// name is subject to change, for example, create a subgraph of the original graph, we should only change name
	int id;			// id is never changed
	int index;		// for indexing
	std::vector<arc*> arclist;
};


struct planargraph {
	vertex *vertices;	// the set of vertices
	arc *arcs;			// the set of arcs
	int n = 0;
	int m = 0;

	planargraph(int nv, std::vector<std::vector<int>> & embedding);
	void create_arc_indices();
	arc create_arc(int sourceindex, int sinkindex);	// add an arc with a source and a sink
	vertex null_vertex();
};

planargraph::planargraph(int nv, std::vector<std::vector<int>> & embedding) {
	n = nv;
	vertices = new vertex[nv];
	for (int i = 0; i < n; i++) {
		vertices[i].name = i;
		vertices[i].index = i;
		vertices[i].id = i;
	}
	// read the embedding
	for (std::vector<std::vector<int>>::iterator it = embedding.begin(); it != embedding.end(); ++it) {
		m += (*it).size();
	}
	arcs = new arc[m]{};
	int u = 0;
	vertex u_vertex;
	for (std::vector<std::vector<int>>::iterator it = embedding.begin(); it != embedding.end(); ++it) {
		std::vector<int> rotation_around_u = *it;
		u_vertex = vertices[u];
		printf("%d:\t", u);
		for (std::vector<int>::iterator arc_it = rotation_around_u.begin(); arc_it != rotation_around_u.end(); ++arc_it) {
			printf("%d\t", *arc_it);
		}
		printf("\n");
		u++;
	}
}

void planargraph::create_arc_indices() {
	for (int i = 0; i <m; i++) {
		arcs[i].index = i;
	}
}

arc planargraph::create_arc(int sourceindex, int sinkindex) {
	arc a {};
	a.source = &vertices[sourceindex];
	a.sink = &vertices[sinkindex];
	a.nextarc = nullptr;
	a.prevarc = nullptr;
	a.rev = nullptr;
	a.mark = false;
	return a;
}

vertex planargraph::null_vertex() {
	vertex v{};
	v.id = -1;
	return v;
}


