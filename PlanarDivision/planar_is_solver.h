#pragma once
#include "planargraph.h"
#include "stdafx.h"

/*
 * An independent set solver for planar graphs which is basically a backtracking program
 * Global variables of the backtracking program must be carefully maintained to guarantee correctness
 */
struct planar_is_solver {
	planargraph *g;
	int branch_count = 0;
	int solution_size = 0;

	// current graph statistics
	int curr_num_active_vertices = 0;
	int *degs;	// deleted vertices have deg -1
	arc** sample_active_arcs;		// a sample arc for each vertex
	planar_is_solver(planargraph *arg_g) : g(arg_g) {
		curr_num_active_vertices = g->n;
		degs = new int[g->n];
		sample_active_arcs = new arc*[g->n];
		for (int i = 0; i < curr_num_active_vertices; i++) {
			degs[i] = g->vertices[i].arclist.size();
			sample_active_arcs[i] = g->vertices[i].arclist.front();
		}
	}

	void find_independent_set() {
		if (curr_num_active_vertices <= 1) {
			std::cout << "Finish " << branch_count << "-th branch" << std::endl;
			return;
		}
		std::stack<int> to_be_deactivated;
		std::stack<vertex *> deg_atmost_2_vertices;
		int deg_min_vertex;
		int deg_min = g->n;
		int deg_max_vertex;
		int deg_max = 0;
		int deg_v;
		// we keep track of deactivate edges during branching
		std::stack<arc*> edge_stack;

		for (int v = 0; v < g->n; v++) {
			if (degs[v] >= 0 && degs[v] <= 2) {
				deg_atmost_2_vertices.push(&g->vertices[v]);
			}
		}
		vertex *v, *u, *w;
		int v_index, u_index, w_index;
		while (!deg_atmost_2_vertices.empty()) {
			v = deg_atmost_2_vertices.top();
			deg_atmost_2_vertices.pop();
			v_index = v->index;
			if (degs[v_index] < 0) continue;	// v is deactivated
			if (degs[v_index] == 0) {
				deactivate_vertex(v_index, edge_stack);
				solution_size++;
			}
			else if(degs[v_index] == 1){
				solution_size++;
				arc *vu = sample_active_arcs[v->index];
				// since v is in the solution, u will be removed from the graph
				// iterate over neighbors of u
				// detect whether the removal of u will result in deg-at-most-2 vertices or not
				arc *ait = vu->rev->nextarc;
				int vit_index = ait->sink->index;
				while (vit_index != v_index) {
					if (degs[vit_index] <= 3) {			// vit has deg at most 2 after removing u
						deg_atmost_2_vertices.push(ait->sink);
					}
					ait = ait->nextarc;
					vit_index = ait->sink->index;
				}
				deactivate_vertex(v_index, edge_stack);
				deactivate_vertex(vu->sink->index, edge_stack);
			}
			else {	// v has degree 2
				arc *uv = sample_active_arcs[v_index]->rev;
				arc *wv = sample_active_arcs[v_index]->nextarc->rev;
				u = uv->source;
				w = wv->source;
				u_index = u->index;
				w_index = w->index;
				//iterate over neighbors of w
				// deactivate edges of w that share neighbors with N[u]
				arc *ait = wv->nextarc;
				int vit_index = ait->sink->index;
				while (vit_index != v_index) {
					if (vit_index == u_index) {
						deactivate_edge(ait);
						edge_stack.push(ait);
					}
					else if (g->edge_exists(u, ait->sink)) {
						deactivate_edge(ait);
						edge_stack.push(ait);
						if (degs[vit_index] <= 3) {	// vit has degree 2 after the removal of the edge (w-vit)
							deg_atmost_2_vertices.push(ait->sink);
						}
					}
					ait = ait->nextarc;
					vit_index = ait->sink->index;
				}
				if (degs[w_index] == 1) {	
					// w has v as the only neighbor left
					deactivate_vertex(w_index, edge_stack);			
				}
				else if (degs[u_index] == 1) { 
					// u has v as the only neighbor left
					deactivate_vertex(u_index, edge_stack);
				}
				else {
					// contract v and w to u
					arc *ux = uv->prevarc;
					arc *uy = uv->nextarc;
					arc *wa = wv->nextarc;
					arc *wb = wv->prevarc;
					ux->nextarc = wa;
					wa->prevarc = ux;
					uy->prevarc = wb;
					wb->nextarc = uy;
					sample_active_arcs[u_index] = ux;
					degs[u_index] = degs[u_index] + degs[w_index] - 2;
					if (degs[u_index] == 2) deg_atmost_2_vertices.push(u);
					w->name = u_index;
					degs[w_index] = degs[u_index];
				}
				deactivate_vertex(v_index, edge_stack);
			}
		}
		print();
	}

	void deactivate_vertex(int v, std::stack<arc*> &edge_stack) {
		// symbolically remove a vertex v from g
		if (degs[v] > 0) {
			deactivate_edge(sample_active_arcs[v]);
			edge_stack.push(sample_active_arcs[v]);
			int u = sample_active_arcs[v]->sink->index;
			arc *it_arc = sample_active_arcs[v]->nextarc;
			while (it_arc->sink->index != u) {
				deactivate_edge(it_arc);
				edge_stack.push(it_arc);
				it_arc = it_arc->nextarc;
			}
		}
		degs[v] = -1;
	}
	void reactivate_vertex(int v) {
		// retore vertex v back to g
	}

	// deactive an edge from the graph
	// need to update sample_active_arc
	void deactivate_edge(arc *uv) {
		int u = uv->source->index;
		int v = uv->sink->index;
		degs[u]--;
		degs[v]--;
		uv->mark = true;
		arc *prv = uv->prevarc;
		arc *nxt = uv->nextarc;
		prv->nextarc = nxt;
		nxt->prevarc = prv;
		sample_active_arcs[u] = prv;

		arc *rev = uv->rev;
		rev->mark = true;
		prv = rev->prevarc;
		nxt = rev->nextarc;
		prv->nextarc = nxt;
		nxt->prevarc = prv;
		sample_active_arcs[v] = prv;
	}
	// reactive an edge from the graph
	void reactivate_edge(arc *uv) {

	}
	~planar_is_solver() {
		delete[] degs;
		delete[] sample_active_arcs;
	}

	void print() {
		printf("*******************************************\n");
		printf("Adjacency list:\n");
		for (int i = 0; i < g->n; i++) {
			if (degs[i] < 0) continue;
			printf("(%d,%d):\t", i, g->vertices[i].id);
			for (std::vector<arc*>::iterator arc_it = g->vertices[i].arclist.begin(); arc_it != g->vertices[i].arclist.end(); ++arc_it) {
				if ((*arc_it)->mark) continue;	// the arc is deactive
				printf("(%d,%d)\t", (*arc_it)->sink->index, (*arc_it)->sink->id);
			}
			printf("\n");
		}
		printf("*******************************************\n");
	}

};
