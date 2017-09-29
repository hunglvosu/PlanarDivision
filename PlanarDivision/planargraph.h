#pragma once
#include<iostream>
#include <vector>
#include<map>
#include<stdio.h>

typedef struct vertex vertex;
typedef struct arc arc;
//struct planargraph;


struct arc {
	vertex *source;
	vertex *sink;
	arc *nextarc;	// the next arc in the clock-wise rotational system
	arc *prevarc;	// the previous arc in the clock-wise rotational system 
	arc *rev;		// the rever of the arc
	int id;
	int index;
	bool mark;

	bool operator==(const arc &other);
};

bool arc::operator==(const arc &other) {
	return id == other.id;
}

struct vertex {
	int name;		// name is subject to change, for example, create a subgraph of the original graph, we should only change name
	int id;			// id is never changed
	int index;		// for indexing
	std::vector<arc*> arclist;

	bool operator==(const vertex &other);
};

bool vertex::operator==(const vertex &other) {
	return id == other.id;
}

struct planargraph {
	vertex *vertices;	// the set of vertices
	arc *arcs;			// the set of arcs
	int n = 0;
	int m = 0;
	std::map<std::pair<int, int>, int> arc_map;	// is a red back tree, it has running time O(log n), avoid using this as much as possible

	planargraph(int nv, std::vector<std::vector<int>> & embedding);
	void create_arc_indices();
	arc create_arc(int sourceindex, int sinkindex);	// add an arc with a source and a sink
	vertex null_vertex();
	arc null_arc();

	void check_rotational_system();
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
	for (int i = 0; i < m; i++) {
		arcs[i] = null_arc();	// initialize null to evry arc
	}
	int u = 0;
	vertex u_vertex;
	vertex v_vertex;
	int arc_index = 0;
	arc uv_arc;
	int uv_arc_index = -1;
	int vu_arc_index = -1;
	for (std::vector<std::vector<int>>::iterator it = embedding.begin(); it != embedding.end(); ++it) {
		std::vector<int> rotation_around_u = *it;
		u_vertex = vertices[u];
		//printf("%d:\t", u);
		//printf("deg:%d\t", rotation_around_u.size());
		arc *prev_arc = &arcs[arc_index + rotation_around_u.size() - 1]; //  the last arc in the rotation system of u
		for (std::vector<int>::iterator arc_it = rotation_around_u.begin(); arc_it != rotation_around_u.end(); ++arc_it) {
			//printf("%d\t", *arc_it);
			v_vertex = vertices[*arc_it];
			//uv_arc = create_arc(u_vertex.index, v_vertex.index);
			arcs[arc_index].source = &vertices[u_vertex.index]; 
			arcs[arc_index].sink = &vertices[v_vertex.index];
			arcs[arc_index].id = arc_index;
			arcs[arc_index].index = arc_index;
			// update prev arc of uv in the rotation around u
			uv_arc_index = arc_index;
			arcs[uv_arc_index].prevarc = prev_arc;
			prev_arc->nextarc = &arcs[uv_arc_index];
			// update the neighbor list of u
			vertices[u].arclist.push_back(&arcs[uv_arc_index]);
			// update prev arc and arc index
			prev_arc = &arcs[uv_arc_index];
			arc_index++;
			// update the rev pointers
			std::pair<int, int> uv_pair(u_vertex.index, v_vertex.index);
			std::pair<int, int> vu_pair(v_vertex.index, u_vertex.index);
			arc_map[uv_pair] = uv_arc_index;	// put index of u-to-v arc to the map
			if (v_vertex.index < u_vertex.index) {	
				// update the reverse pointer of u->v and v->u
				vu_arc_index = arc_map[vu_pair];
				arcs[uv_arc_index].rev = &arcs[vu_arc_index];
				arcs[vu_arc_index].rev = &arcs[uv_arc_index];
			}
		}
		//printf("\n");
		u++;
	}
	check_rotational_system();
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

arc planargraph::null_arc() {
	arc a{};
	a.source = nullptr;
	a.sink = nullptr;
	a.id = -1;
	a.nextarc = nullptr;
	a.prevarc = nullptr;
	a.rev = nullptr;
	a.mark = false;
	return a;
}

void planargraph::check_rotational_system() {
	printf("\nChecking the rotational system\n");
	printf("Forward check\n");
	for (int i = 0; i < n; i++) {
		printf("%d:\t", i);
		arc *arc_it = vertices[i].arclist.front();
		while (arc_it->mark != true) {
			printf("%d\t", arc_it->sink->id);
			arc_it->mark = true;
			arc_it = arc_it->nextarc;
		}
		printf("\n");
	}
	for (int i = 0; i < m; i++) {
		arcs[i].mark = false;
	}
	printf("Backward check\n");
	for (int i = 0; i < n; i++) {
		printf("%d:\t", i);
		arc *arc_it = vertices[i].arclist.front();
		while (arc_it->mark != true) {
			printf("%d\t", arc_it->sink->id);
			arc_it->mark = true;
			arc_it = arc_it->prevarc;
		}
		printf("\n");
	}
	for (int i = 0; i < m; i++) {
		arcs[i].mark = false;
	}
	printf("Reverse check\n");
	for (int i = 0; i < m; i++) {
		printf("arc:(%d,%d) and its rev: (%d,%d)\n", arcs[i].source->id, arcs[i].sink->id, arcs[i].rev->source->id, arcs[i].rev->sink->id);
	}
	printf("Checking done!\n");
}
