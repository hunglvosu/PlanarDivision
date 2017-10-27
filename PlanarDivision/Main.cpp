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
#include <vector>
#include <ctime> // time_t
#include <unordered_map>

int main() {
	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(4);
	planargraph g(4*4, embedding);
	r_division(g, 5);
//	benchmarking();
	getchar();
	return 0;
}

