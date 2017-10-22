#pragma once
#include "dual_tree.h"
#include "dfs_visitor.h"
#include "reversible_list.h"
// dfs the dual tree to find the separation edge

struct separation_edge_locator :  dfs_visitor{

	dual_tree dual_bfs_tree;
	int *dual_vertex_index_to_arc_index;	// one arc is sufficient to indentify a face
	int *arc_index_to_dual_vertex_index;
	bool *primal_tree_arc_marker;

	// supporting data that can be deleted after execution
	int *inside_count;	
	bool *is_visited;
	srlist<int>* cycles;
	srlist<int>** cycle_ptrs;

	separation_edge_locator(dual_tree const & arg_dual_tree) : dual_bfs_tree(arg_dual_tree) {
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

		int degree_of_u = u->arclist.size();

		if (degree_of_u <= 1) {
			// v is the parent of u in the dfs tree
			arc *uv_arc = u->arclist[0];
			arc *primal_uv_arc = uv_arc->nextarc;	// primal_uv_arc defines the dual face of u
			cycles[u->index].push_back(primal_uv_arc->sink->index);
			cycles[u->index].push_back(primal_uv_arc->rev->prevarc->sink->index);
			cycles[u->index].push_back(primal_uv_arc->source->index);
			//printf("cycle at leaf:\n");
			//cycles[u->index].print();
			cycle_ptrs[u->index] = &cycles[u->index];
		}
		else if (degree_of_u == 2) {
			// v is the only child of u in the dfs tree
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
			arc** children_arc_ptrs = new arc*[2];
			int j = 0;
			for (int i = 0; i < u->arclist.size(); i++) {
				if (is_visited[u->arclist[i]->sink->index]) {
					children_arc_ptrs[j] = u->arclist[i];
					j++;
				}
			}
			arc* left_child = children_arc_ptrs[0];
			arc *right_child = children_arc_ptrs[1];
			if (children_arc_ptrs[1]->sink->index == children_arc_ptrs[0]->source->index) {
				right_child = left_child;
				left_child = children_arc_ptrs[0];
			}
			printf("children of %d is: %d\t%d\n", u->index, left_child->sink->index, right_child->sink->index);
			srlist<int>* cycle_ptr_left = cycle_ptrs[left_child->sink->index];
			srlist<int>* cycle_ptr_right = cycle_ptrs[right_child->sink->index];

			int last_equal;
			int p = 0;
			while ((*cycle_ptr_left).front() == (*cycle_ptr_right).back()) {
				last_equal = (*cycle_ptr_left).front();
				(*cycle_ptr_left).remove_front();
				(*cycle_ptr_right).remove_back();
				p++;
			}
			// push back the last vertex that are the same for both v and w
			(*cycle_ptr_left).push_front(last_equal);
			(*cycle_ptr_right).push_back(last_equal);
			(*cycle_ptr_right).splice((*cycle_ptr_left));
			cycle_ptrs[u->index] = cycle_ptr_right;
			inside_count[u->index] = inside_count[left_child->sink->index] + inside_count[right_child->sink->index] + p - 1;
		}
		is_visited[u->index] = true;
	}

};

void find_low_radius_separator(dual_tree dual_bfs_tree) {
	separation_edge_locator edge_locator(dual_bfs_tree);
	dfs(&dual_bfs_tree.vertices[0], dual_bfs_tree, edge_locator);
}
