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
#include <vector>
#include <ctime> // time_t
#include <unordered_map>
#include <time.h>
#include <string.h>
#include <cstdlib>

using namespace std;
vector<vector<int>> create_sample_3x3_grid_embedding();
vector<vector<int>> create_sample_deg_1_embedding();
vector<vector<int>> create_sample_grid_nxn_embedding(int n);
vector<vector<int>> create_sample_star_1xn_embedding(int n);
vector<vector<int>> create_special_embedding();
void benchmarking();
void test_smart_pointer();
struct sample_face_visitor;
struct sample_bfs_visitor;
struct sample_dfs_visitor;

struct sample_dfs_visitor : dfs_visitor {
	void discover_vertex(vertex *u) {
		//printf("Encounter %d for the first time\n", u->id);
	}
	
	void examine_arc(arc *uv) {
		//printf("Visit arc %d->%d\n", uv->source->id, uv->sink->id);
	}
	
	void tree_arc(arc *uv) {
		//printf("Tree arc %d->%d\n", uv->source->id, uv->sink->id);
	}
	
	void back_arc(arc *uv) {
		//printf("Back arc %d->%d\n", uv->source->id, uv->sink->id);
	}

	void forward_or_cross_arc(arc *uv) {
		//printf("Forward or cross arc %d->%d\n", uv->source->id, uv->sink->id);
	}
	
	void finish_vertex(vertex *u) {
		//printf("All vertices reachable from %d are visited\n", u->id);
	}
	
};
struct sample_bfs_visitor : bfs_visitor {
	void discover_vertex(vertex *u) {
		//printf("Put %d to the queue\n", u->id);
	}

	void examine_vertex(vertex *u) {
		//printf("Pop %d from the queue\n", u->id);
	}
	void tree_arc(arc *uv) {
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

};
struct sample_face_visitor : face_traversal_visitor {
	void begin_traversal() {
		printf("Begin face traversal\n");
	}
	void begin_face() {
		//printf("Traverse a new face\n");
	}
	void next_vertex(vertex *v) {
		//printf("Process vertex %d\n", v->name);
	}

	void next_arc(arc *uv) {
		//printf("Process arc %d->%d\n", uv->source->name, uv->sink->name);
	}

	void end_face() {
		//printf("End traversing a face\n");
	}
	void end_traversal() {
		printf("End face traversal\n");
	}

};

int main() {
	
	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(4);
	planargraph g(16, embedding);
//	planar_triangulate(g);
//	g.reset();
//	g.print();
//	g.check_rotational_system();
	compute_r_division(g, 4);
	//g.print();
//	vector<vector<int>> embedding = create_special_embedding();
//	planargraph g(5, embedding);
//	std::vector<int> separator_container;
//	find_low_radius_separator(g, separator_container);
	
	//find_separator(g, separator_container);
/*	printf("Separator size %ld\n", separator_container.size());
	for (int i = 0; i < separator_container.size(); i++) {
	printf("%d\t", separator_container[i]);
	}
	printf("\n");
	*/
//	bfs_tree primal_bfs_tree(g, &g.vertices[0]);
//	bfs(&g.vertices[0], g, primal_bfs_tree);
	//primal_bfs_tree.print();
//	dual_tree dual_bfs_tree(primal_bfs_tree);
//	dual_tree_builder tree_buider(dual_bfs_tree);
//	planar_face_traversal(g, tree_buider);
	//dual_bfs_tree.print();
	//dual_bfs_tree.print_dual_faces();
//	find_low_radius_separator(dual_bfs_tree);
	
//	vector<vector<int>> embedding = create_sample_deg_1_embedding();
//	planargraph g(4, embedding);

//	vector<vector<int>> embedding = create_sample_star_1xn_embedding(5);
//	planargraph g(5, embedding);
	//planar_triangulate(g);
//	sample_bfs_visitor vis;
//	bfs(&g.vertices[0], g, vis);
//	sample_dfs_visitor vis;
//	dfs(&g.vertices[0], g, vis);
//	g.check_rotational_system();
//	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(3);
//	planar_triangulate(g);
//	g.check_rotational_system();
//	sample_face_visitor face_visitor;
//	planar_face_traversal(g, face_visitor);
//	srlist<int> sample_list;
//	sample_list.debug();

//	benchmarking();
	getchar();
	return 0;
}

void test_smart_pointer() {
	int *arr = new int[100000000];
	//std::unique_ptr<int[]> arr(new int[100000000]);
	for (int i = 0; i < 100; i++) arr[i] = i + 1;
	for (int i = 0; i < 100; i++) {
		std::cout << arr[i];
	}
	delete[] arr;
}
void benchmarking() {
	time_t begin, end;
	printf("creating the embedding");
	time(&begin);
	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(1000);
	time(&end);
	double difference = difftime(end, begin);
	printf("time taken for creating 1 mil vertices %.2lf seconds.\n", difference);
	printf("reading the embedding");
	time(&begin);
	planargraph g(1000000, embedding);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken for reading the planar embedding of 1 mil vertices %.2lf seconds.\n", difference);
	printf("reading complete");
	sample_face_visitor face_visitor;
	time(&begin);
	planar_face_traversal(g, face_visitor);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken for visiting all faces %.2lf seconds.\n", difference);

	time(&begin);
	int arc_index;
	for (int i = 0; i < g.m; i++) {
		arc_index = g.arc_map.find(g.arc_to_int64(g.arcs[i].source, g.arcs[i].sink))->second;
	}
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to read arc_map %.2lf seconds.\n", difference);


	time(&begin);
	planar_triangulate(g);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to triangulate 1 mil g %.2lf seconds.\n", difference);
	
	time(&begin);
	sample_bfs_visitor bfs_vis;
	bfs(&g.vertices[0], g, bfs_vis);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to bfs 1 mil g %.2lf seconds.\n", difference);

	time(&begin);
	sample_dfs_visitor dfs_vis;
	dfs(&g.vertices[0], g, dfs_vis);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to dfs 1 mil g %.2lf seconds.\n", difference);

	/*time(&begin);
	bfs_tree primal_bfs_tree(g, &g.vertices[0]);
	bfs(&g.vertices[0], g, primal_bfs_tree);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to build a primal bfs tree of mil g %.2lf seconds.\n", difference);

	time(&begin);
	dual_tree dual_bfs_tree(primal_bfs_tree);
	dual_tree_builder tree_buider(dual_bfs_tree);
	planar_face_traversal(g, tree_buider);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to build a dual bfs tree of mil g %.2lf seconds.\n", difference);*/

	time(&begin);
	std::vector<int> separator_container;
	find_low_radius_separator(g, separator_container);
	time(&end);
	difference = difftime(end, begin);
	printf("time taken to find a separator of mil g %.2lf seconds.\n", difference);

}

// this planar graph  is smallest that allows one to test all four cases of low radius separator algorithm
vector<vector<int>> create_special_embedding() {
	vector<vector<int>> embedding_storage;
	vector<int> rot0;
	rot0.push_back(2); rot0.push_back(3); rot0.push_back(1);
	embedding_storage.push_back(rot0);

	vector<int> rot1;
	rot1.push_back(0); rot1.push_back(3); rot1.push_back(4); rot1.push_back(2);
	embedding_storage.push_back(rot1);

	vector<int> rot2;
	rot2.push_back(1); rot2.push_back(4); rot2.push_back(3); rot2.push_back(0);
	embedding_storage.push_back(rot2);

	vector<int> rot3;
	rot3.push_back(0); rot3.push_back(2); rot3.push_back(4); rot3.push_back(1);
	embedding_storage.push_back(rot3);

	vector<int> rot4;
	rot4.push_back(3); rot4.push_back(2); rot4.push_back(1); 
	embedding_storage.push_back(rot4);
	return embedding_storage;

}

vector<vector<int>> create_sample_3x3_grid_embedding() {
	vector<vector<int>> embedding_storage;
	vector<int> rot0;
	rot0.push_back(1); rot0.push_back(3);
	embedding_storage.push_back(rot0);

	vector<int> rot1;
	rot1.push_back(2); rot1.push_back(4); rot1.push_back(0);
	embedding_storage.push_back(rot1);

	vector<int> rot2;
	rot2.push_back(5); rot2.push_back(1);
	embedding_storage.push_back(rot2);

	vector<int> rot3;
	rot3.push_back(0); rot3.push_back(4); rot3.push_back(6);
	embedding_storage.push_back(rot3);

	vector<int> rot4;
	rot4.push_back(1); rot4.push_back(5); rot4.push_back(7); rot4.push_back(3);
	embedding_storage.push_back(rot4);

	vector<int> rot5;
	rot5.push_back(2); rot5.push_back(8); rot5.push_back(4);
	embedding_storage.push_back(rot5);

	vector<int> rot6;
	rot6.push_back(3); rot6.push_back(7);
	embedding_storage.push_back(rot6);

	vector<int> rot7;
	rot7.push_back(4); rot7.push_back(8); rot7.push_back(6);
	embedding_storage.push_back(rot7);

	vector<int> rot8;
	rot8.push_back(5); rot8.push_back(7);
	embedding_storage.push_back(rot8);

	return embedding_storage;
}

vector<vector<int>> create_sample_deg_1_embedding() {
	vector<vector<int>> embedding_storage;
	vector<int> rot0;
	rot0.push_back(1);
	embedding_storage.push_back(rot0);

	vector<int> rot1;
	rot1.push_back(2); rot1.push_back(3); rot1.push_back(0);
	embedding_storage.push_back(rot1);

	vector<int> rot2;
	rot2.push_back(3); rot2.push_back(1);
	embedding_storage.push_back(rot2);

	vector<int> rot3;
	rot3.push_back(1); rot3.push_back(2);
	embedding_storage.push_back(rot3);

	return embedding_storage;
}

vector<vector<int>> create_sample_grid_nxn_embedding(int n) {
	vector<vector<int>> embedding_storage;
	for (int i = 0; i < n; i++) {
		/*if (i % 10 == 0) {
			printf("Read %d\n", i*n);
		}*/
		for (int j = 0; j < n; j++) {
			vector<int> rot;
			rot.reserve(4);
			if (i >= 1) rot.push_back((i - 1)*n + j);
			if (j + 1 < n) rot.push_back(i*n + j + 1);
			if (i + 1 < n) rot.push_back((i + 1)*n + j);
			if (j >= 1) rot.push_back(i*n + j -1);
			embedding_storage.push_back(rot);
		}
	}
	return embedding_storage;
}

vector<vector<int>> create_sample_star_1xn_embedding(int n) {
	vector<vector<int>> embedding_storage;
	for (int i = 0; i < n - 1; i++) {
		vector<int> rot;
		rot.push_back(n - 1);
		embedding_storage.push_back(rot);
	}
	vector<int> rot_n_1;
	for (int i = 0; i < n - 1; i++) {
		rot_n_1.push_back(i);
	}
	embedding_storage.push_back(rot_n_1);
	return embedding_storage;

}