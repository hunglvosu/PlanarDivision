#pragma once
#include "planar_separator.h"
#include "planar_triangulator.h"

typedef std::vector<vertex*> vertex_container;
typedef std::vector<arc*> arc_container;

void create_subplanargraph(planargraph &g_subgraph, graph_components &g_components, int comp_id) {
	for (int i = 0; i < g_subgraph.n; i++) {
		g_subgraph.vertices[i].id =  g_components.vertices_of_components[comp_id][i]->id;	// recall id never change
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

void compute_r_division(planargraph &g, int r) {
	int sep_count = 0;
	planar_triangulate(&g);
	//int *separator_container = new int[g.n];
	std::vector<int> boundary_vertices;
	std::vector<int> separator_container;
	find_low_radius_separator(&g, separator_container);
	boundary_vertices.insert(boundary_vertices.end(), separator_container.begin(), separator_container.end());
	printf("%d-th separation\n", sep_count);
	sep_count++;
	g.reset();
	// find subgraphs of g after removing the separator
	graph_components g_components;
	g_components.init(&g);
	separtor_bfs(g, g_components, separator_container);
	std::list<planargraph> big_graph_lists;	// list of subgraphs that have more than r vertices
	std::list<planargraph> small_graph_lists;// list of subgraphs that have at most r vertices
	for (int comp_id = 0; comp_id < g_components.num_components; comp_id++) {
		if (g_components.vertices_of_components[comp_id].size() > r) {
			// the component is big
			big_graph_lists.push_back(planargraph());
			big_graph_lists.back().init(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
			create_subplanargraph(big_graph_lists.back(), g_components, comp_id);
			big_graph_lists.back().init_arc_map();
		}
		else {
			small_graph_lists.push_back(planargraph());
			small_graph_lists.back().init(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
			create_subplanargraph(small_graph_lists.back(), g_components, comp_id);
		}	
	}

	
	while (!big_graph_lists.empty()) {
		//printf("processsing a big graph\n");
		planar_triangulate(&big_graph_lists.back());
		//big_graph_lists.back().print();
		separator_container.clear();
		find_low_radius_separator(&big_graph_lists.back(), separator_container);
		boundary_vertices.insert(boundary_vertices.end(), separator_container.begin(), separator_container.end());
		if(sep_count % 10 == 2)printf("%d-th separation\n", sep_count);
		sep_count++;
		big_graph_lists.back().reset();
		// clear g_components and relcaim the mamory
		graph_components g_components(&big_graph_lists.back());
		separtor_bfs(big_graph_lists.back(), g_components, separator_container);
		for (int comp_id = 0; comp_id < g_components.num_components; comp_id++) {
			if (g_components.vertices_of_components[comp_id].size() > r) {
				// the component is big
				big_graph_lists.push_front(planargraph());
				big_graph_lists.front().init(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
				create_subplanargraph(big_graph_lists.front(), g_components, comp_id);
				//big_graph_lists.front().print();
			}
			else {
				small_graph_lists.push_back(planargraph());
				small_graph_lists.back().init(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
				create_subplanargraph(small_graph_lists.back(), g_components, comp_id);
			}
		}
		big_graph_lists.pop_back();

	}
	/*while (!small_graph_lists.empty()) {
		printf("small component\n");
		for (int i = 0; i < small_graph_lists.back().n; i++) {
			printf("%d\t", small_graph_lists.back().vertices[i].id);
		}
		printf("\n");
		small_graph_lists.pop_back();
	}*/
	printf("boundary size of the r-division: %d\n", boundary_vertices.size());
	printf("Separation at %d quality = %lf\n", r, 1.0 - (double)boundary_vertices.size()/g.n);
	
	// try to generate a planar graph from the first component
	//int comp_id = 1;
	//planargraph g_subgraph(g_components.vertices_of_components[comp_id].size(), g_components.arcs_of_components[comp_id].size());
	//create_subplanargraph(g_subgraph, g_components, comp_id);

}

