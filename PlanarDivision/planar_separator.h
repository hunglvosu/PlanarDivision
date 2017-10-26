#pragma once
#include "low_radius_separator.h"

void find_separator(planargraph &g, std::vector<int> &separator_container) {
	bfs_tree primal_bfs_tree(&g, &g.vertices[0]);
	bfs(&g.vertices[0], g, primal_bfs_tree);
	int sqrt_n = ((int)sqrt(g.n));
	// Find a median level i such that L[0] + ... + L[i-1] <= n/2 and L[0]+...+ L[i] > n/2
	// Here, L[i] is the number of vertices at level i
	int i = -1;
	int *L = new int[g.n];
	for (int i = 0; i < g.n; i++) {
		L[i] = 0;
	}
	int maxLevel = 0;
	for (int i = 0; i < g.n; i++) {
		L[primal_bfs_tree.levels[i]] ++;
		maxLevel = (maxLevel < primal_bfs_tree.levels[i]) ? primal_bfs_tree.levels[i] : maxLevel;
	}
	if (maxLevel <= (int)sqrt(2 * g.n)) {
		find_low_radius_separator(&g, separator_container);
	}
	else {
		int Li = 0;	// Li = L[0] +... + L[i]
		int half_n = g.n / 2;
		int med_level = 0;
		for (int i = 0; i <= maxLevel; i++) {
			if (Li + L[i] <= half_n) {
				Li += L[i];
				med_level++;
			}
			else {
				break;
			}
		}
		std::cout << "Median level: " << med_level << std::endl;
		if (L[med_level] <= 2 * sqrt_n) {
			// found a separator
			for (int i = 0; i < g.n; i++) {
				if (primal_bfs_tree.levels[i] == med_level) {
					//separator_container.push_back(i);
					return;
				}
			}
		}
		int k = Li + L[med_level];
		int two_sqrt_k = (int)(2 * sqrt(k));
		int two_sqrt_n_k = (int)(2 * sqrt(g.n - k));
		int l0 = med_level;
		while (l0 >= 0) {
			if (L[0] + 2 * (med_level - l0) <= two_sqrt_k) break;
			l0--;
		}
		int l2 = med_level + 1;
		while (l2 <= maxLevel) {
			if (L[l2] + 2 * (l2 - med_level - 1) <= two_sqrt_n_k) break;
			l2--;
		}
		// push vertices of l0 and l2 to the separator
		for (int i = 0; i <= g.n; i++) {
			if (primal_bfs_tree.levels[i] == l0 || primal_bfs_tree.levels[i] == l2) {
				//separator_container.push_back(i);
			}
		}
		// contract vertices of l1 into a new vertex
		std::cout << "l0 = " << l0 << "\t l2= "<< l2<< std::endl;
	}
	
}