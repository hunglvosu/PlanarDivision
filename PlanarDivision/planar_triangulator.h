#pragma once
#include "planargraph.h"
#include "planar_face_visitor.h"
#include <iostream>
#include <vector>

struct  triangulator : face_traversal_visitor
{

	std::vector<vertex*> vertices_on_face;
	int *visited_times;	// count how many time we have visited a vertex on the same face
	planargraph &g;
	int num_arcs_stor_in_g = 0;

	triangulator(planargraph &arg_g) : g(arg_g) {
		visited_times = new int[g.n];
		for (int i = 0; i < g.n; i++) visited_times[i] = 0;
		num_arcs_stor_in_g = g.m;
	};

	void begin_traversal() {
		printf("Begin face traversal\n");
	}
	void begin_face() {
		vertices_on_face.reserve(16);
		//printf("Traverse a new face\n");
	}
	void next_vertex(vertex* v) {
		
		//printf("Process vertex %d\n", v.name);
	}

	void next_arc(arc* uv) {
		vertex *u = uv->source;
		vertex *v = uv->sink;
		printf("visit arc %d->%d\n", u->index, v->index);
		visited_times[u->index]++;
		if (visited_times[u->index] > 1) {
			printf("%d is visited at least twice\n", u->index);
			vertex *w = vertices_on_face.back();
			printf("add two arcs: %d->%d and %d->%d\n", v->index, w->index, v->index, w->index);
			g.arcs[num_arcs_stor_in_g] = g.create_arc(v->index, w->index);
			g.arcs[num_arcs_stor_in_g + 1] = g.create_arc(w->index, v->index);
			num_arcs_stor_in_g += 2;
		}
		vertices_on_face.push_back(u);

		//printf("Process arc %d->%d\n", uv.source->name, uv.sink->name);
	}

	void end_face() {
		for (int i = 0; i < vertices_on_face.size(); i++) {
			visited_times[vertices_on_face[i]->index] = 0;
		}
		vertices_on_face.clear();
		//printf("End traversing a face\n");
	}
	void end_traversal() {
		printf("End face traversal\n");
	}

};

void planar_triangulator(planargraph &g){
	triangulator trg(g);
	planar_face_traversal(g, trg);
}
