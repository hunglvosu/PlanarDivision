#pragma once
#include "bfs_visitor.h"
#include "planar_face_visitor.h"

typedef std::vector<vertex*> vertex_container;
typedef std::vector<arc*> arc_container;

struct face_counter : face_traversal_visitor {
	planargraph *g;
	int face_count = 0;
	face_counter(planargraph *arg_g) : g(arg_g) {
		face_count = 0;
	}
	void begin_traversal() {}
	void begin_face() {
		face_count++;
	}
	void next_vertex(vertex* v) {}
	virtual void next_arc(arc *uv) {}
	virtual void end_face() {}
	virtual void end_traversal() {}

};

// check a planar embedding is valid or not from Euler formular
void check_planar_embedding(planargraph &g) {
	int n = g.n;
	int m = g.m/2; // recall m = # arcs which is twice the number of edges
	face_counter f_counter(&g);
	planar_face_traversal(&g, f_counter);
	int f = f_counter.face_count;
	if (n - m + f == 2) {
		std::cout << "The embedding is planar" << std::endl;
	}
	else {
		std::cout << "The embedding is non-planar: (n,m,f) = (" << n << "," << m <<"," << f << ")" <<  std::endl;
	}
}

// to check triangulation
struct triangular_face_checker : face_traversal_visitor {
	planargraph *g;
	int face_length = 0;
	triangular_face_checker(planargraph *arg_g) : g(arg_g) {}
	void begin_traversal() {}
	void begin_face() {
		face_length = 0;
	}
	void next_vertex(vertex* v) {}
	virtual void next_arc(arc *uv) {
		face_length++;
	}
	virtual void end_face() {
		if (face_length != 3) {
			std::cout << "There is a non-triangular face" << std::endl;
		}
	}
	virtual void end_traversal() {}

};

void check_triangulation(planargraph &g) {
	check_planar_embedding(g);
	triangular_face_checker t_checker(&g);
	planar_face_traversal(&g, t_checker);
	std::cout << "Checking triangulation done!" << std::endl;
}

void check_parallel_edges(planargraph &g) {
	std::unordered_map<__int64, int> map;
	__int64 arc_code = 0;
	for (int i = 0; i < g.m; i++) {
		arc_code = g.arc_to_int64(g.arcs[i].source, g.arcs[i].sink);
		if (map.find(arc_code) != map.end()) {
			std::cout << "THERE ARE PARALLEL ARCS" << std::endl;
			return;
		}
		else {
			map.insert(std::unordered_map<__int64, int>::value_type(arc_code, i)); // put u->v arc to the map		
		}
	}
	//std::cout << "There are no parallel arcs" << std::endl;
}

struct graph_components : vertex_excluding_bfs_visitor {
	planargraph *g;
	int num_components;
	std::vector<vertex_container> vertices_of_components;
	std::vector<arc_container> arcs_of_components;
	vertex_container sources_of_components;
	// the current component source vertex
	vertex *current_source;
	int current_comp_index = -1;
	int *vertex_to_comp_id;

	graph_components(planargraph *arg_g) {
		init(arg_g);
	}
	graph_components() {
		vertex_to_comp_id = nullptr;
		num_components = 0;
	}
	~graph_components() {
		//printf("destruct graph component\n");
		delete[] vertex_to_comp_id;
	}

	void init(planargraph *arg_g) {
		g = arg_g;
		vertex_to_comp_id = new int[g->n];
		for (int i = 0; i < g->n; i++) {
			vertex_to_comp_id[i] = -1;
		}
		num_components = 0;
	}
	void new_component(vertex *u) {
		//printf("new component start at %d\n", u->index);
		current_source = u;
		current_comp_index++;
		sources_of_components.push_back(u);
		vertex_to_comp_id[u->index] = current_comp_index;
	}
	// the design of separator_bfs_visitor gurantee that both u and v are not in the separator
	void tree_arc(arc *uv) {
		vertex *u = uv->source;
		vertex *v = uv->sink;
		vertex_to_comp_id[v->index] = vertex_to_comp_id[u->index];
	}

	void finish_traversal() {
		num_components = current_comp_index + 1;
		//printf("#components = %d\n", num_components);
		for (int i = 0; i < num_components; i++) {
			vertices_of_components.push_back(vertex_container());
			arcs_of_components.push_back(arc_container());
		}
		int comp_id = -1;
		for (int i = 0; i < g->n; i++) {
			comp_id = vertex_to_comp_id[i];
			if (comp_id >= 0) {
				vertices_of_components[comp_id].push_back(&g->vertices[i]);
			}
		}
		int source_comp_id = -1;
		int sink_comp_id = -1;
		for (int i = 0; i < g->m; i++) {
			source_comp_id = vertex_to_comp_id[g->arcs[i].source->index];
			sink_comp_id = vertex_to_comp_id[g->arcs[i].sink->index];
			if (source_comp_id >= 0 && sink_comp_id >= 0) {
				arcs_of_components[source_comp_id].push_back(&g->arcs[i]);
			}
		}
		/*		for (int i = 0; i < num_components; i++) {
		printf("vertices of comp# %d\n", i);
		for (int j = 0; j < vertices_of_components[i].size(); j++) {
		printf("%d\t", vertices_of_components[i][j]->index);
		}
		printf("\n");
		printf("arcs of comp# %d\n", i);
		for (int j = 0; j < arcs_of_components[i].size(); j++) {
		printf("%d->%d\t",arcs_of_components [i][j]->source->index, arcs_of_components[i][j]->sink->index);
		}
		printf("\n");
		}*/
	};

	void discover_vertex(vertex *u) {}
	void examine_vertex(vertex *u) {}
	void non_tree_arc(arc *uv) {}
	void gray_sink(arc *uv) {}
	void black_sink(arc *uv) {}
	void finish_vertex(vertex *u) {}
	void finish_component(vertex *u) {}

};

void create_subplanargraph(planargraph &g_subgraph, graph_components &g_components, int comp_id) {
	for (int i = 0; i < g_subgraph.n; i++) {
		g_subgraph.vertices[i].id = g_components.vertices_of_components[comp_id][i]->id;	// recall id never change
		g_components.vertices_of_components[comp_id][i]->name = i; // update the name of the corresponding vertex to update arcs later on
		g_subgraph.vertices[i].index = i;
	}
	// update arcs of the planar subgraph
	vertex *source, *sink;
	for (int i = 0; i < g_subgraph.m; i++) {
		// change the name of the graph to point to the index of the corresponding arc in the subgraph
		// this will help in updating the rotational system of the subgraph
		g_components.arcs_of_components[comp_id][i]->name = i;
		source = &g_subgraph.vertices[g_components.arcs_of_components[comp_id][i]->source->name];
		sink = &g_subgraph.vertices[g_components.arcs_of_components[comp_id][i]->sink->name];
		g_subgraph.arcs[i].source = source;
		g_subgraph.arcs[i].sink = sink;
		g_subgraph.arcs[i].version = 0;
		g_subgraph.arcs[i].mark = false;
		g_subgraph.arcs[i].index = i;
		g_subgraph.arcs[i].name = i;
		source->arclist.push_back(&g_subgraph.arcs[i]);
	}
	//g_subgraph.print();
	// update the nextarc, prevarc and rev pointers
	arc *nextarc, *prevarc;
	int source_comp_id = -1;
	int sink_comp_id = -1;
	for (int i = 0; i < g_subgraph.m; i++) {
		g_subgraph.arcs[i].rev = &g_subgraph.arcs[g_components.arcs_of_components[comp_id][i]->rev->name];
		nextarc = g_components.arcs_of_components[comp_id][i];
		// recall that source and/or sink of nextarc could be in the separator
		do {
			nextarc = nextarc->nextarc;
			source_comp_id = g_components.vertex_to_comp_id[nextarc->source->index];
			sink_comp_id = g_components.vertex_to_comp_id[nextarc->sink->index];
		} while (source_comp_id < 0 || sink_comp_id < 0);
		g_subgraph.arcs[i].nextarc = &g_subgraph.arcs[nextarc->name];

		prevarc = g_components.arcs_of_components[comp_id][i];
		do {
			prevarc = prevarc->prevarc;
			source_comp_id = g_components.vertex_to_comp_id[prevarc->source->index];
			sink_comp_id = g_components.vertex_to_comp_id[prevarc->sink->index];
		} while (source_comp_id < 0 || sink_comp_id < 0);
		g_subgraph.arcs[i].prevarc = &g_subgraph.arcs[prevarc->name];
	}
	//g_subgraph.check_rotational_system();
	//printf("*****************vertex map*****************\n");
	//for (int i = 0; i < g_components.vertices_of_components[comp_id].size(); i++) {
	//	printf("%d = %d\n", g_components.vertices_of_components[comp_id][i]->id, g_components.vertices_of_components[comp_id][i]->name);
	//}
}

// find planar subgraphs of a graph g when a vertex set is removed
// when S = emptyset, this procedure returns connected components of g
void find_planar_subgraphs(planargraph &g, std::vector<int> &S, std::list<planargraph> &subgraph_lists) {
	graph_components g_components;
	g_components.init(&g);
	vertex_ecluding_bfs(g, g_components, S);
	for (int comp_id = 0; comp_id < g_components.num_components; comp_id++) {
		subgraph_lists.push_back(planargraph());
		subgraph_lists.back().init(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
		create_subplanargraph(subgraph_lists.back(), g_components, comp_id);
		subgraph_lists.back().init_arc_map();
	}
}

void find_planar_connected_components(planargraph &g, std::list<planargraph> &subgraph_lists) {
	std::vector<int> S; // S = emptyset
	std::cout << "Find connected components" << std::endl;
	find_planar_subgraphs(g, S, subgraph_lists);
	std::cout << "The # of connected componets = " << subgraph_lists.size() << std::endl;
}

