#pragma once
#include "bfs_tree.h"
#include "planargraph.h"
#include "planar_face_visitor.h"

/*
*	In constructing the dual tree of the bfs tree of a planar graph, we make use the fact that each arc of a planar graph
*	is associated with exactly one face. Thus, each non-tree arc of G can be used to identify a corresponding face of the dual tree
*	We assume that the input graph is triangulated
*/
struct  dual_tree : graph
{
	int *dual_vertex_index_to_arc_index;	// one arc is sufficient to indentify a face
	int *arc_index_to_dual_vertex_index;
	planargraph g;
	dual_tree( planargraph const &arg_g, int arg_n) : g(arg_g) {
		n = arg_n;
		vertices = new vertex[n];
		for (int i = 0; i < n; i++) {
			vertices[i].name = i;
			vertices[i].index = i;
			vertices[i].id = i;
		}
		arcs = new arc[n-1];
		dual_vertex_index_to_arc_index = new int[n];
		arc_index_to_dual_vertex_index = new int[g.m];
	}

};

struct dual_tree_builder : face_traversal_visitor {
	dual_tree dtree;
	int current_face_index = -1;
	dual_tree_builder(dual_tree &arg_dtree) : dtree(arg_dtree) {};
	
	void begin_traversal() {
		//printf("Begin face traversal\n");
	}
	void begin_face() {
		current_face_index++;
		//printf("Traverse a new face\n");
	}
	void next_vertex(vertex *v) {
		//printf("Process vertex %d\n", v->name);
	}

	void next_arc(arc *uv) {
		dtree.arc_index_to_dual_vertex_index[uv->index] = current_face_index;
		dtree.dual_vertex_index_to_arc_index[current_face_index] = uv->index;
		//printf("Process arc %d->%d\n", uv->source->name, uv->sink->name);
	}

	void end_face() {
		//printf("End traversing a face\n");
	}
	void end_traversal() {
		printf("End face traversal\n");
	}

};