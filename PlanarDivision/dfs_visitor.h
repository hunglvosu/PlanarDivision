#pragma once
#include "planargraph.h"
#include <stack>
// we adopt visitor class definition of boost
// see http://www.boost.org/doc/libs/1_65_1/libs/graph/doc/DFSVisitor.html for more details
struct dfs_visitor {

	virtual void discover_vertex(vertex *u) {};
	virtual void examine_arc(arc *uv) {};
	virtual void tree_arc(arc *uv) {};
	virtual void back_arc(arc *uv) {};
	virtual void forward_or_cross_arc(arc *uv) {};	
	virtual void finish_vertex(vertex *u) {};	// when all out-going arcs of u are examined
};

void bfs(vertex *source, planargraph &g, dfs_visitor &vis) {
	int *color = new int[g.n];
	for (int i = 0; i < g.n; i++) {
		color[i] = g.white;
	}
	vis.discover_vertex(source);
	recursive_dfs(source, g, vis, color);
	delete color;
}

void recursive_dfs(vertex *u, planargraph &g, dfs_visitor &vis, int *color) {
	color[u->index] = g.gray;
	arc *uv;
	vertex *v;
	for (std::vector<arc*>::iterator it = g.vertices[u->index].arclist.begin(); it != g.vertices[u->index].arclist.end(); ++it) {
		uv = *it;
		vis.examine_arc(uv);
		v = uv->sink;
		if (color[v->index] == g.white) {
			vis.discover_vertex(v);
			color[v->index] = g.gray;
			vis.tree_arc(uv);
			recursive_dfs(v, g, vis, color);
		}
		else if (color[v->index] == g.gray) {
			vis.back_arc(uv);
		}
		else {	// color[v] = black
			vis.forward_or_cross_arc(uv);
		}
	}
	color[u->index] = g.black;
	vis.finish_vertex(u);
}
/*
void bfs(vertex *source, planargraph &g, dfs_visitor &vis) {
	int *color = new int[g.n];
	int white = 0, gray = 1, black = 2;
	for (int i = 0; i < g.n; i++) {
		color[i] = white;
	}
	std::stack<vertex *> C;
	vis.discover_vertex(source);
	C.push(source);
	color[source->index] = gray;
	vertex *u;
	vertex *v;
	arc *uv;
	while (!C.empty())
	{
		u = C.top();
		C.pop();
		color[u->index] = black;
		for (std::vector<arc*>::iterator it = g.vertices[u->index].arclist.begin(); it != g.vertices[u->index].arclist.end(); ++it) {
			uv = *it;
			v = uv->sink;
			vis.examine_arc(uv);
			if (color[v->index] == white) {
				C.push(v);
				vis.discover_vertex(v);
				color[v->index] = gray;
				vis.tree_arc(uv);
			}
			else {

			}
		}

	}
	delete color;
}
*/