#include <iostream>
#include "planargraph.h"
#include "stdafx.h"
#include <vector>

using namespace std;
vector<vector<int>> create_sample_embedding();

int main() {
	
	vector<vector<int>> embedding = create_sample_embedding();
	planargraph g(9, embedding);
	getchar();
	return 0;
}

vector<vector<int>> create_sample_embedding() {
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