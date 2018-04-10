#include <iostream>
#include "planargraph.h"
#include "planar_face_visitor.h"
#include "planar_triangulator.h"
#include "bfs_visitor.h"
#include "dfs_visitor.h"
#include "bfs_tree.h"
#include "stdafx.h"
#include "dual_tree.h"
#include "low_radius_separator.h"
#include "planar_separator.h"
#include "reversible_list.h"
#include "r_divisor.h"
#include "unit_test.h"
#include "utils.h"
#include "FVS_kernel.h"
#include "planar_is_solver.h"
#include "graph_utils.h"




int main(int argc, char *argv[]) {
//	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(10);
//	planargraph g(5 * 5, embedding);
//	vector<vector<int>> embedding = create_sample_star_1xn_embedding(4);
//	planargraph g(4, embedding);
//	g.print();
	//planar_is_solver solver(&g);
	//solver.find_independent_set();
	//write_division_from_input_embedding(argc, argv);
//	std::vector<std::vector<int>> embedding_storage;
//	read_embedding_from_file(argv[1], embedding_storage);
//	int r = std::stoi(argv[2]);
//	planargraph g(embedding_storage.size(), embedding_storage);
/*	kdiv div;
	r_division(g, r, div);
	for (auto kg : div.graphs) {
		print_k_graph(kg);
	}*/
	

	std::vector<std::vector<int>> embedding_storage;
	read_embedding_from_file("graph_300k", embedding_storage);
	int r = 10;
	std::cout << "Finish reading from file" << std::endl;
	planargraph g(embedding_storage.size(), embedding_storage);
	std::cout << "Finish creating a planar graph" << std::endl;
	std::list<planargraph> subgraphs;
	find_planar_connected_components(g, subgraphs);
	int i = 0;
	while (!subgraphs.empty()) {
		std::cout << "Comp # " << i << " (m,n) = ("<< subgraphs.back().n<< "," << subgraphs.back().m/2 << ")" << std::endl;
		//planar_triangulate(&subgraphs.back());
		//check_parallel_edges(subgraphs.back());
		r_division(subgraphs.back(), r);
		subgraphs.pop_back();
		i++;
	}
	//r_division(g, r);
	getchar();
	return 0;
}

