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

int main(int argc, char *argv[]) {
	std::vector<std::vector<int>> embedding_storage;
	read_embedding_from_file(argv[1], embedding_storage);
	int r = std::stoi(argv[3]);
	planargraph g(embedding_storage.size(), embedding_storage);
	std::vector<int> boundary_vertices;
	std::list<planargraph> small_graph_lists;
	r_division(g, r, boundary_vertices, small_graph_lists);
	write_output(boundary_vertices, small_graph_lists, argv[2]);
//	benchmarking();
//	getchar();
	return 0;
}

