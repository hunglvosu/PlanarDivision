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





int main(int argc, char *argv[]) {
//	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(10);
//	planargraph g(10 * 10, embedding);
	std::vector<std::vector<int>> embedding_storage;
	read_embedding_from_file(argv[1], embedding_storage);
	int r = std::stoi(argv[2]);
	planargraph g(embedding_storage.size(), embedding_storage);
	kdiv div;
	r_division(g, r, div);
	for (auto kg : div.graphs) {
		print_k_graph(kg);
	}
//	benchmarking();
	getchar();
	return 0;
}

