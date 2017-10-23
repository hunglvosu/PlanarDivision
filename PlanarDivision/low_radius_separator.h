#pragma once
#include "dual_tree.h"
#include "dfs_visitor.h"
#include "reversible_list.h"
// dfs the dual tree to find the separation edge

struct separation_edge_locator : dfs_visitor {

	dual_tree dual_bfs_tree;
	int *dual_vertex_index_to_arc_index;	// one arc is sufficient to indentify a face
	int *arc_index_to_dual_vertex_index;
	bool *primal_tree_arc_marker;

	// supporting data that can be deleted after execution
	int *inside_count;
	bool *is_visited;
	srlist<int>* cycles;
	srlist<int>** cycle_ptrs;
	bool is_separator_found = false;
	std::vector<int>* separator_container;

	separation_edge_locator(dual_tree const & arg_dual_tree, std::vector<int> *arg_separator_container) : dual_bfs_tree(arg_dual_tree), separator_container(arg_separator_container) {
		arc_index_to_dual_vertex_index = dual_bfs_tree.arc_index_to_dual_vertex_index;
		dual_vertex_index_to_arc_index = dual_bfs_tree.dual_vertex_index_to_arc_index;
		primal_tree_arc_marker = dual_bfs_tree.primal_tree.tree_arc_marker;

		inside_count = new int[dual_bfs_tree.n];
		is_visited = new bool[dual_bfs_tree.n];
		for (int i = 0; i < dual_bfs_tree.n; i++) {
			inside_count[i] = 0;
			is_visited[i] = false;
		}
		cycles = new srlist<int>[dual_bfs_tree.n];
		cycle_ptrs = new srlist<int>*[dual_bfs_tree.n];
	};
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

	/*	void finish_vertex(vertex *u) {
		/*printf("finish: %d\n", u->index);
		printf("face:\n");
		arc *uv = &dual_bfs_tree.primal_tree.g.arcs[dual_vertex_index_to_arc_index[u->index]];
		dual_bfs_tree.print_arc(uv);
		arc *current;
		for (current = uv->rev->prevarc; current != uv; current = current->rev->prevarc) {
			dual_bfs_tree.print_arc(current);
		}
		int degree_of_u = u->arclist.size();

		arc **arcs_on_face = new arc*[3]; // set of arcs on the dual face of u
		arcs_on_face[0] = &dual_bfs_tree.primal_tree.g.arcs[dual_vertex_index_to_arc_index[u->index]];
		arcs_on_face[1] = arcs_on_face[0]->rev->prevarc;
		arcs_on_face[2] = arcs_on_face[1]->rev->prevarc;

		if (degree_of_u <= 1) {
			//arc *uv = &dual_bfs_tree.primal_tree.g.arcs[dual_vertex_index_to_arc_index[u->index]];
			//arc *vx = uv->rev->prevarc;
			//arc *xu = vx->rev->prevarc;
			arc *xy = nullptr;
			for (int i = 0; i < 3; i++) {
				if ((!primal_tree_arc_marker[arcs_on_face[i]->index]) && (!primal_tree_arc_marker[arcs_on_face[i]->rev->index])) {
					xy = arcs_on_face[i];
				}
			}
			vertex *z = xy->rev->prevarc->sink;
			inside_count[u->index] = 0;
			printf("Case 1 at %d\n", u->index);
			cycles[u->index].push_back(xy->source->index);
			cycles[u->index].push_back(z->index);
			cycles[u->index].push_back(xy->sink->index);
			//printf("cycle at leaf:\n");
			//cycles[u->index].print();
			cycle_ptrs[u->index] = &cycles[u->index];
		}
		else if (degree_of_u == 2) {
			//printf("Case 2 or Case 3 at %d\n", u->index);
			// v  is the children of u
//			arc *vu_arc = is_visited[u->arclist[0]->index] ? u->arclist[0]->rev : u->arclist[1]->rev;
//			vertex *v = vu_arc->source;
//			arc* primal_vu_arc = vu_arc->prevarc;
//			arc* primal_uv_arc = primal_vu_arc->rev;
			arc *uv_arc = is_visited[u->arclist[0]->index] ? u->arclist[0] : u->arclist[1];
			vertex *v = uv_arc->sink;
			arc *primal_uv_arc = uv_arc->prevarc;
			srlist<int>* v_cycle_ptr = cycle_ptrs[v->index];
			int x_index = (*v_cycle_ptr).front();
			(*v_cycle_ptr).remove_front();
			int y_index = (*v_cycle_ptr).back();
			(*v_cycle_ptr).remove_back();

			vertex *z = primal_uv_arc->rev->sink;
			if (z->index = (*v_cycle_ptr).front()) {
				printf("Case 3 at %d\n", u->index);
				(*v_cycle_ptr).push_back(y_index);	// push y back to the cycle
				cycle_ptrs[u->index] = v_cycle_ptr;
			}
			else if (z->index == (*v_cycle_ptr).back()) {
				printf("Case 3 at %d\n", u->index);
				(*v_cycle_ptr).push_back(x_index); // push x back to the cycle
				cycle_ptrs[u->index] = v_cycle_ptr;
			}
			else {
				printf("Case 2 at %d\n", u->index);
				// need to decide whether to put z to the front or back of the cycle corresponding to v

			}
		}
		else {
			// degree_of_u == 3
			printf("Case 4 at %d\n", u->index);
			// find children of u in the dfs tree
			vertex **u_children = new vertex*[2];
			int j = 0;
			for (int i = 0; i < u->arclist.size(); i++) {
				if (is_visited[u->arclist[i]->sink->index]) {
					u_children[j] = u->arclist[i]->sink;
					j++;
				}
			}
			printf("children of %d is: %d\t%d\n",u->index, u_children[0]->index, u_children[1]->index);
			srlist<int>* v_cycle_ptr = cycle_ptrs[u_children[0]->index];
			srlist<int>* w_cycle_ptr = cycle_ptrs[u_children[1]->index];
			// gurantee that v_cycle.back() == w_cycle.front()
			if ((*v_cycle_ptr).front() == (*w_cycle_ptr).front()) {
				(*v_cycle_ptr).reverse();
			}
			else if ((*v_cycle_ptr).front() == (*w_cycle_ptr).back()) {
				(*v_cycle_ptr).reverse();
				(*w_cycle_ptr).reverse();
			}
			else if ((*v_cycle_ptr).back() == (*w_cycle_ptr).back()) {
				(*w_cycle_ptr).reverse();
			}
			int last_equal;
			int p = 0;
			while ((*v_cycle_ptr).back() == (*w_cycle_ptr).front()) {
				last_equal = (*v_cycle_ptr).back();
				(*v_cycle_ptr).remove_back();
				(*w_cycle_ptr).remove_front();
				p++;
			}
			// push back the last vertex that are the same for both v and w
			(*v_cycle_ptr).push_back(last_equal);
			(*v_cycle_ptr).splice(*w_cycle_ptr);
			(*v_cycle_ptr).print();
			cycle_ptrs[u->index] = v_cycle_ptr;
			inside_count[u->index] = inside_count[u_children[0]->index] + inside_count[u_children[1]->index] + p - 1;
		}
		is_visited[u->index] = true;
	}*/

	void finish_vertex(vertex *u) {

		if (is_separator_found) return;
		int degree_of_u = u->arclist.size();

		if (degree_of_u <= 1) {
			// v is the parent of u in the dfs tree
			arc *uv_arc = u->arclist[0];
			arc *primal_uv_arc = uv_arc->nextarc;	// primal_uv_arc defines the dual face of u
			cycles[u->index].push_back(primal_uv_arc->sink->index);
			cycles[u->index].push_back(primal_uv_arc->rev->prevarc->sink->index);
			cycles[u->index].push_back(primal_uv_arc->source->index);
			cycle_ptrs[u->index] = &cycles[u->index];
		}
		else if (degree_of_u == 2) {
			// v is the only child of u in the dfs tree
			arc *uv_arc = is_visited[u->arclist[0]->sink->index] ? u->arclist[0] : u->arclist[1];
			vertex *v = uv_arc->sink;
			arc *primal_vu_arc = uv_arc->rev->nextarc;
			srlist<int>* v_cycle_ptr = cycle_ptrs[v->index];
			int x_index = (*v_cycle_ptr).front();
			(*v_cycle_ptr).remove_front();
			int y_index = (*v_cycle_ptr).back();
			(*v_cycle_ptr).remove_back();

			// note here that primal_vu_arc->sink = x = (*v_cycle_ptr).front()
			//				  primal_vu_arc->source = y = (*v_cycle_ptr).back()
			arc *xz = primal_vu_arc->rev->nextarc;
			vertex *z = xz->sink;

			if (primal_tree_arc_marker[xz->index] || primal_tree_arc_marker[xz->rev->index]) {
				// xz or zx is a tree arc
				if (z->index == (*v_cycle_ptr).front()) {
					//printf("Case 3a at %d, z = %d\n", u->index, z->index);
					(*v_cycle_ptr).push_back(y_index);	// push y to the back of the cycle
					cycle_ptrs[u->index] = v_cycle_ptr;
					inside_count[u->index] = inside_count[v->index] + 1;
				}
				else {
					//printf("Case 2a at %d\n", u->index);
					(*v_cycle_ptr).push_back(y_index);
					(*v_cycle_ptr).push_front(x_index);
					(*v_cycle_ptr).push_front(z->index);
					cycle_ptrs[u->index] = v_cycle_ptr;
					inside_count[u->index] = inside_count[v->index];
				}
			}
			else {
				// yz or zy is a tree arc
				if (z->index == (*v_cycle_ptr).back()) {
					//printf("Case 3b at %d\n", u->index);
					(*v_cycle_ptr).push_front(x_index); // push x to the front of the cycle
					cycle_ptrs[u->index] = v_cycle_ptr;
					inside_count[u->index] = inside_count[v->index] + 1;
				}
				else {
					//printf("Case 2b at %d\n", u->index);
					(*v_cycle_ptr).push_back(y_index);
					(*v_cycle_ptr).push_front(x_index);
					(*v_cycle_ptr).push_back(z->index);
					cycle_ptrs[u->index] = v_cycle_ptr;
					inside_count[u->index] = inside_count[v->index];
				}
			}
		}
		else {
			// degree_of_u == 3
			//printf("Case 4 at %d\n", u->index);
			// find children of u in the dfs tree
			arc** children_arc_ptrs = new arc*[2];
			int j = 0;
			for (int i = 0; i < u->arclist.size(); i++) {
				if (is_visited[u->arclist[i]->sink->index]) {
					children_arc_ptrs[j] = u->arclist[i];
					j++;
				}
			}
			arc* left_child = children_arc_ptrs[0];
			arc *dual_left_child = left_child->rev->nextarc;
			arc *right_child = children_arc_ptrs[1];
			arc *dual_right_child = right_child->rev->nextarc;
			if (dual_left_child->sink->index != dual_right_child->source->index) {
				right_child = left_child;
				left_child = children_arc_ptrs[1];
			}
			srlist<int>* cycle_ptr_left = cycle_ptrs[left_child->sink->index];
			srlist<int>* cycle_ptr_right = cycle_ptrs[right_child->sink->index];
			//(*cycle_ptr_left).print();
			//(*cycle_ptr_right).print();
			int last_equal = -1;
			int p = 0;
			while ((*cycle_ptr_left).front() == (*cycle_ptr_right).back()) {
				last_equal = (*cycle_ptr_left).front();
				(*cycle_ptr_left).remove_front();
				(*cycle_ptr_right).remove_back();
				p++;
			}
			// push back the last vertex that are the same for both v and w
			(*cycle_ptr_left).push_front(last_equal);
			(*cycle_ptr_right).splice((*cycle_ptr_left));
			cycle_ptrs[u->index] = cycle_ptr_right;
			inside_count[u->index] = inside_count[left_child->sink->index] + inside_count[right_child->sink->index] + p - 1;
		}
		//printf("*******************cycle at %d**********************:\n", u->index);
		//(*cycle_ptrs[u->index]).print();
		//printf("inside count %d\n", inside_count[u->index]);
		//printf("|c(e)| = %d\n", (*cycle_ptrs[u->index]).size());
		is_visited[u->index] = true;
		int ng = dual_bfs_tree.primal_tree.g.n;
		int param = (2 * ng) / 3;
		if (inside_count[u->index] <= param && (ng - inside_count[u->index] - (*cycle_ptrs[u->index]).size()) <= param) {
			// found a good separating edge
			is_separator_found = true;
			int u_index = u->index;
			while (!(*cycle_ptrs[u_index]).is_empty()) {
				(*separator_container).push_back((*cycle_ptrs[u_index]).back());
				(*cycle_ptrs[u_index]).remove_back();
			}
		}
	}

};

void find_low_radius_separator(dual_tree dual_bfs_tree) {
	std::vector<int> separator_container;
	separation_edge_locator edge_locator(dual_bfs_tree, &separator_container);
	dfs(&dual_bfs_tree.vertices[0], dual_bfs_tree, edge_locator);
}

void find_low_radius_separator(planargraph &g) {
	bfs_tree primal_bfs_tree(g, &g.vertices[0]);
	bfs(&g.vertices[0], g, primal_bfs_tree);
	//primal_bfs_tree.print();
	dual_tree dual_bfs_tree(primal_bfs_tree);
	dual_tree_builder tree_buider(dual_bfs_tree);
	planar_face_traversal(g, tree_buider);
	//dual_bfs_tree.print();
	//dual_bfs_tree.print_dual_faces();

	std::vector<int> separator_container;
	separation_edge_locator edge_locator(dual_bfs_tree, &separator_container);
	// find a leaf of the dual tree to be the start vertex of dfs
	int s = 0;
	for (int i = 0; i < dual_bfs_tree.n; i++) {
		if (dual_bfs_tree.vertices[i].arclist.size() == 1) s = i;
	}
	dfs(&dual_bfs_tree.vertices[s], dual_bfs_tree, edge_locator);
	printf("Separator size %ld\n", separator_container.size());
	/*for (int i = 0; i < separator_container.size(); i++) {
		printf("%d\t", separator_container[i]);
	}
	printf("\n");*/
}