#pragma once
#include "planargraph.h"
#include <queue>

// we adopt visitor class definition of boost
// see http://www.boost.org/doc/libs/1_46_1/libs/graph/doc/BFSVisitor.html for more details
struct bfs_visitor {
	
	virtual void discover_vertex(vertex *u);
	virtual void examine_vertex(vertex *u);
	virtual void examin_arc(arc *uv);
	virtual void tree_arc(arc *uv);
	virtual void non_tree_arc(arc *uv);
	virtual void gray_sink(arc *uv);	// equivalent to gray_target of boost
	virtual void black_sink(arc *uv);
	virtual void finish_vertex(vertex *u);
};

void bfs(vertex *source, planargraph &g, bfs_visitor &vis) {
	int *color = new int[g.n];	
	int white = 0, gray = 1, black = 2;
	// 0 = while, 1 = gray, 2 = black
	for (int i = 0; i < g.n; i++) {
		color[i] = white;
	}
	std::queue<vertex*> vqueue;
	vis.discover_vertex(source);
	vqueue.push(source);
	color[source->index] = gray;
	vertex *u;
	vertex *v;
	arc *uv;
	while (!vqueue.empty())
	{
		u = vqueue.front();
		vqueue.pop();
		if (color[u->index] == black) continue;
		vis.examine_vertex(u);
		for (std::vector<arc*>::iterator it = g.vertices[u->index].arclist.begin(); it != g.vertices[u->index].arclist.end(); ++it){
			uv = *it;
			v = uv->sink;
			vis.examin_arc(uv);
			if (color[v->index] == white) {
				vqueue.push(v);
				color[v->index] = gray;
				vis.tree_arc(uv);
			}
			else {
				if (color[v->index] == gray) {
					vis.gray_sink(uv);
				}
				else {	// color of v must be black
					vis.black_sink(uv);
				}
				vis.non_tree_arc(uv);
			}
		} // end examine neighbors of u
		color[u->index] = black;
		vis.finish_vertex(u);
	}
	delete color;
}