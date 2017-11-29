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
//#include "planar_is_solver.h"


std::list<std::vector<int>> create_sample_3x3_grid_Boyer_embedding() {
	std::list<std::vector<int>> embedding_storage;
	std::vector<int> rot0;
	rot0.push_back(0); rot0.push_back(1); rot0.push_back(3);
	embedding_storage.push_back(rot0);

	std::vector<int> rot1;
	rot1.push_back(1); rot1.push_back(2); rot1.push_back(4); rot1.push_back(0);
	embedding_storage.push_back(rot1);

	std::vector<int> rot2;
	rot2.push_back(2); rot2.push_back(5); rot2.push_back(1);
	embedding_storage.push_back(rot2);

	std::vector<int> rot3;
	rot3.push_back(3); rot3.push_back(0); rot3.push_back(4); rot3.push_back(6);
	embedding_storage.push_back(rot3);

	std::vector<int> rot4;
	rot4.push_back(4); rot4.push_back(1); rot4.push_back(5); rot4.push_back(7); rot4.push_back(3);
	embedding_storage.push_back(rot4);

	std::vector<int> rot5;
	rot5.push_back(5); rot5.push_back(2); rot5.push_back(8); rot5.push_back(4);
	embedding_storage.push_back(rot5);

	std::vector<int> rot6;
	rot6.push_back(6); rot6.push_back(3); rot6.push_back(7);
	embedding_storage.push_back(rot6);

	std::vector<int> rot7;
	rot7.push_back(7); rot7.push_back(4); rot7.push_back(8); rot7.push_back(6);
	embedding_storage.push_back(rot7);

	std::vector<int> rot8;
	rot8.push_back(8); rot8.push_back(5); rot8.push_back(7);
	embedding_storage.push_back(rot8);

	return embedding_storage;
}


void r_division(std::list<std::vector<int>> Boyer_embedding, int r, kdiv &div){
	// convert Boyer_embedding to vector embedding
	std::vector<std::vector<int>> embedding_storage;
	for(int i = 0; i < Boyer_embedding.size(); i++){
		embedding_storage.push_back(std::vector<int>());
	}
	for(std::list<std::vector<int>>::iterator it = Boyer_embedding.begin(); it != Boyer_embedding.end(); ++it){
		int i = (*it).front();
		for(int j = 1 ; j < (*it).size(); j++){
			embedding_storage[i].push_back((*it)[j]);
		}
	}
	planargraph g(embedding_storage.size(), embedding_storage);
	std::vector<int> boundary_vertices;
	std::list<planargraph> small_graph_lists;
	r_division(g, r, boundary_vertices, small_graph_lists);
	to_kernel_rDiv(div, boundary_vertices, small_graph_lists);	
}



int main(int argc, char *argv[]) {
	//vector<vector<int>> embedding = create_sample_star_1xn_embedding(4);
	//planargraph g(4, embedding);
//	write_division_from_input_embedding(argc, argv); 

/*	std::vector<std::vector<int>> embedding_storage;
	read_embedding_from_file(argv[1], embedding_storage);
	int r = std::stoi(argv[2]);
	planargraph g(embedding_storage.size(), embedding_storage);
	kdiv div;
	r_division(g, r, div);
	for (auto kg : div.graphs) {
		print_k_graph(kg);
	}*/
	
	std::list<std::vector<int>> Boyer_embedding = create_sample_3x3_grid_Boyer_embedding();
	int r = 4;
	kdiv div;
	r_division(Boyer_embedding, r, div);
	for (auto kg : div.graphs) {
		print_k_graph(kg);
	}
	return 0;
}

