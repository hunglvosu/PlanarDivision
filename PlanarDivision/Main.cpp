#include <iostream>
#include "planargraph.h"
#include "planar_face_visitor.h"
#include "stdafx.h"
#include <vector>
#include <ctime> // time_t


using namespace std;
vector<vector<int>> create_sample_3x3_grid_embedding();
vector<vector<int>> create_sample_deg_1_embedding();
vector<vector<int>> create_sample_grid_nxn_embedding(int n);
struct sample_face_visitor;

struct sample_face_visitor : face_traversal_visitor {
	void begin_traversal() {
		printf("Begin face traversal\n");
	}
	void begin_face() {
		//printf("Traverse a new face\n");
	}
	void next_vertex(vertex v) {
		//printf("Process vertex %d\n", v.name);
	}

	void next_arc(arc uv) {
		//printf("Process arc %d->%d\n", uv.source->name, uv.sink->name);
	}

	void end_face() {
		//printf("End traversing a face\n");
	}
	void end_traversal() {
		printf("End face traversal\n");
	}

};

int main() {
	
//	vector<vector<int>> embedding = create_sample_3x3_grid_embedding();
//	vector<vector<int>> embedding = create_sample_deg_1_embedding();
	vector<vector<int>> embedding = create_sample_grid_nxn_embedding(3);
	planargraph g(19, embedding);
	sample_face_visitor face_visitor;
	planar_face_traversal(g, face_visitor);
/*	time_t begin, end;
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
	printf("time taken for visiting all faces %.2lf seconds.\n", difference);*/
	getchar();
	return 0;
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