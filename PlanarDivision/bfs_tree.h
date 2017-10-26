#pragma once
#include "planargraph.h"
#include "bfs_visitor.h"

struct bfs_tree : bfs_visitor {

	graph *g;
	int*  levels;
	bool* tree_arc_marker;
	vertex *root;

	bfs_tree(graph *arg_g, vertex *arg_root) : g(arg_g), root(arg_root) {
		levels = new int[g->n];
		for (int i = 0; i < g->n; i++) levels[i] = -1;
		levels[root->index] = 0;
		tree_arc_marker = new bool[g->m];
		for (int i = 0; i < g->m; i++) tree_arc_marker[i] = false;
	};

	~bfs_tree() {
		//printf("destruct bfs primal tree\n");
		delete[] levels;
		delete[] tree_arc_marker;
	}

	void discover_vertex(vertex *u) {
		//printf("Put %d to the queue\n", u->id);
	}

	void examine_vertex(vertex *u) {
		//printf("Pop %d from the queue\n", u->id);
	}
	void tree_arc(arc *uv) {
		tree_arc_marker[uv->index] = true;
		levels[uv->sink->id] = levels[uv->source->id] + 1;
		//printf("Tree arc %d->%d\n", uv->source->id, uv->sink->id);
	}

	void non_tree_arc(arc *uv) {
		//printf("Non tree arc %d->%d when examining neighbors of %d\n", uv->source->id, uv->sink->id, uv->source->id);
	}

	void gray_sink(arc *uv) {
		//printf("Neighbor %d of %d is not yet visited and in queue\n", uv->source->id, uv->sink->id);
	}

	void black_sink(arc *uv) {
		//printf("Neighbor %d of %d is visited\n", uv->source->id, uv->sink->id);
	}
	void finish_vertex(vertex *u) {
		//printf("All neighbor of %d is put in queue or visited\n", u->id);
	}

	void print() {
		std::cout << "Levels: \n" << std::endl;
		for (int i = 0; i < g->n; i++) std::cout << "level[" << i << "] = " << levels[i] << std::endl;
		std::cout << "Tree arcs: \n" << std::endl;
		for (int i = 0; i < g->m; i++) {
			if (tree_arc_marker[i]) {
				g->print_arc(&g->arcs[i]);
			}
		}
	}

};
