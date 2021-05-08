// 1, 2, 5, 7, 8, 10, 11, 13

#include "queue.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <climits>

enum generate_mode{directed, undirected};
struct GraphNode {
	GraphNode* next;
	std::size_t end_vertex;
	int weight;

	GraphNode(std::size_t end_vertex, int weight, GraphNode* next = nullptr) {
		this->next = next;
		this->end_vertex = end_vertex;
		this->weight = weight;
	}

};

struct Path {
	bool is_exist;
	std::size_t lenght;
	std::vector<std::size_t> path;
};


struct AdjMatrix {
	int** matrix;
	std::size_t size;

	AdjMatrix(std::size_t size) {
		this->size = size;
		this->matrix = new int* [size];

		for (std::size_t i = 0; i < size; i++) {
			this->matrix[i] = new int[size];
			for (std::size_t j = 0; j < size; j++) {
				this->matrix[i][j] = 0;
			}
		}
	}

	void add_edge(std::size_t start, std::size_t end, int weight) {
		assert(start < this->size);
		assert(end < this->size);
		assert(this->matrix[start][end] == 0 && "Try to replace existing edge");
		this->matrix[start][end] = weight;
	}

	void remove_edge(std::size_t start, std::size_t end) {
		assert(start < this->size);
		assert(end < this->size);
		assert(this->matrix[start][end] != 0 && "Try to remove unexisting edge");
		this->matrix[start][end] = 0;
	}
	void print_matrix() {
		for (std::size_t i = 0; i < size; i++) {
			for (std::size_t j = 0; j < size; j++) {
				std::cout << this->matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	void add_undirected_edge(std::size_t start, std::size_t end, int weight) {
		assert(start < this->size);
		assert(end < this->size);

		assert(this->matrix[start][end] == 0 && "Try to replace existing edge");
		assert(this->matrix[end][start] == 0 && "Try to replace existing edge");

		add_edge(start, end, weight);
		add_edge(end, start, weight);
	}

	void remove_undirected_edge(std::size_t start, std::size_t end) {
		assert(start < this->size);
		assert(end < this->size);

		assert(this->matrix[start][end] != 0 && "Try to remove unexisting edge");
		assert(this->matrix[end][start] != 0 && "Try to remove unexisting edge");
		assert(this->matrix[start][end] == this->matrix[end][start] && "Try to remove two different directed edges");

		remove_edge(start, end);
		remove_edge(end, start);
	}

	void print_edges() {
		bool is_empty = true;
		for (std::size_t i = 0; i < size; i++) {
			bool is_edge = false;
			for (std::size_t j = 0; j < size; j++) {
				if (this->matrix[i][j] != 0) {
					std::cout << i << "->" << j << " (" << this->matrix[i][j] << "), ";
					is_edge = true;
					is_empty = false;
				}
			}
			if (is_edge) {
				std::cout << std::endl;
			}
			
		}
		if (is_empty) {
			std::cout << "No edges" << std::endl;
		}
	}

	void transitive_closure() {
		for (std::size_t k = 0; k < this->size; k++) {
			for (std::size_t i = 0; i < this->size; i++) {
				for (std::size_t j = 0; j < this->size; j++) {
					if (this->matrix[i][k] && this->matrix[k][j]) {
						if (i != j) {
							this->matrix[i][j] = rand() % 9 + 1;
						}
					}
				}
			}
		}
	}
	
	/*
	~AdjMatrix() {
		this->size = 0;
		for (std::size_t i = 0; i < size; i++) {
			delete[]this->matrix[i];
		}
		delete []this->matrix;
	}
	*/

	template <typename Callable>
	void depth_search_all(Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		for (std::size_t i = 0; i < this->size; i++) {
			if (already_visited[i] == false) {
				depth_search_impl(i, already_visited, process, process_after, compare_vertixes);
			}
		}
		delete[]already_visited;
	}

	template <typename Callable>
	void depth_search_one_component(std::size_t start_vertex, Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		assert(start_vertex < this->size);

		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		depth_search_impl(start_vertex, already_visited, process, process_after, compare_vertixes);
		delete[]already_visited;
	}

	bool is_connected() {
		for (std::size_t i = 0; i < this->size; i++) {
			std::size_t vertex_number = 0;
			depth_search_one_component(i, [&vertex_number](std::size_t vertex) {vertex_number++; });
			if (vertex_number < this->size) {
				return false;
			}
		}
		return true;
	}

	bool is_cycles() {
//		bool* already_visited = new bool[this->size];
//		for (std::size_t i = 0; i < this->size; i++) {
//			already_visited[i] = false;
//		}

		for (std::size_t i = 0; i < this->size; i++) {

			int* already_visited = new int[this->size];
			for (std::size_t j = 0; j < this->size; j++) {
				already_visited[j] = 0;
			}
//			if (already_visited[i] == false) {
				if (depth_search_impl_parent(i, -1, already_visited) == true) {
					return true;
				}
//			}
			delete[]already_visited;
		}
//		delete[]already_visited;
		return false;
	}

	bool is_tree() {
		return !is_cycles() && is_connected();
	}

	template <typename Callable>
	void breadth_search_all(Callable process, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		for (std::size_t i = 0; i < this->size; i++) {
			if (already_visited[i] == false) {
				breadth_search_impl(i, already_visited, process, compare_vertixes);
			}
		}
		delete[]already_visited;
	}

	template <typename Callable>
	void breadth_search_one_component(std::size_t start_vertex, Callable process, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		assert(start_vertex < this->size);

		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		breadth_search_impl(start_vertex, already_visited, process, compare_vertixes);
		delete[]already_visited;
	}

	/*

	std::vector<Path> get_path_from_one(std::size_t vertex) {
		std::vector <int> prev(this->size, -1);
		std::vector<std::size_t> distances = get_distance(vertex, prev);
		std::vector <Path> paths_struture;

		for (std::size_t i = 0; i < distances.size(); i++) {
			Path a;
			int j = i;
			if (prev[j] != -1) { 
				a.is_exist = true;
				while (j != -1) { // put the path
					a.path.push_back(j);
					j = prev[j];
				}
			}
			else {
				a.is_exist = false;
			}
			reverse(a.path.begin(), a.path.end());

			a.lenght = distances[i]; // put the length

			paths_struture.push_back(a);
		}
		return paths_struture;
	}


	Path get_path_between_two(std::size_t start_vertex, std::size_t end_vertex) {
		std::vector <int> prev(this->size, -1);
		std::vector<std::size_t> distances = get_distance(start_vertex, prev);

		Path a;
		int j = start_vertex;
		if (prev[j] != -1) {
			a.is_exist = true;
			while (j != -1) { // put the path
				a.path.push_back(j);
				j = prev[j];
			}
		}
		else {
			a.is_exist = false;
		}
		reverse(a.path.begin(), a.path.end());

		a.lenght = distances[start_vertex]; // put the length

		return a;
	}

	Path** get_path_from_all() {
		Path** path_matrix = new Path * [this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			path_matrix[i] = new Path[this->size];
		}

		for (std::size_t k = 0; k < this->size; k++) { // path from k to i;
			std::vector <int> prev(this->size, -1);
			std::vector<std::size_t> distances = get_distance(k, prev);

			for (std::size_t i = 0; i < distances.size(); i++) {
				Path a;
				int j = i;
				if (prev[j] != -1) {
					a.is_exist = true;
					while (j != -1) { // put the path
						a.path.push_back(j);
						j = prev[j];
					}
				}
				else {
					a.is_exist = false;
				}
				reverse(a.path.begin(), a.path.end());

				a.lenght = distances[i]; // put the length

				path_matrix[k][i] = a;
			}
		}
		return path_matrix;
	}

	*/

	std::vector<std::size_t> get_distance(std::size_t start_vertex, std::vector <int>& prev) {
		const std::size_t INF = INT_MAX;
		std::vector<std::size_t> distances(this->size, INF);
		distances[start_vertex] = 0;

		std::vector <bool> used(this->size, false);

		std::size_t min_dist = 0;
		std::size_t min_vertex = start_vertex;
		while (min_dist < INF) {
			std::size_t i = min_vertex;
			used[i] = true;

			for (std::size_t j = 0; j < this->size; j++) { // put marks to its neightbors
				if (distances[i] + this->matrix[i][j] < distances[j] && this->matrix[i][j] != 0) {
					distances[j] = distances[i] + this->matrix[i][j];
					prev[j] = i;
				}
			}

			min_dist = INF;
			for (std::size_t j = 0; j < this->size; j++) { // choose the minimum distance
				if (!used[j] && distances[j] < min_dist) {
					min_dist = distances[j];
					min_vertex = j;
				}
			}
		}

		return distances;
	}


	std::vector<std::size_t> topological_sort() {
		// checking if graph has cycles in direct sense
		assert(!is_cycles() && "This graph can't be sorted");
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = 0; j < this->size; j++) {
				if (this->matrix[i][j] != 0) {
					assert(this->matrix[j][i] == 0 && "This graph can't be sorted");
				}
			}
		}
		std::vector<std::size_t> vertices_order;
		depth_search_all([&vertices_order](std::size_t vertex) {vertices_order.push_back(vertex); }, true);
		std::reverse(vertices_order.begin(), vertices_order.end());
		return vertices_order;
		AdjMatrix sorted_graph(this->size);
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = 0; j < this->size; j++) {
				sorted_graph.matrix[i][j] = this->matrix[vertices_order[i]][vertices_order[j]];
			}
		}
		//return sorted_graph;
	}




private:
	template <typename Callable>
	void depth_search_impl(std::size_t start_vertex, bool* already_visited, Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		if (process_after == false) {
			process(start_vertex);
		}
		
		already_visited[start_vertex] = true;

		std::vector<GraphNode> to_visit;
		for (std::size_t i = 0; i < this->size; i++) {
			if (this->matrix[start_vertex][i] != 0) {
				GraphNode new_node(i, this->matrix[start_vertex][i]);
				to_visit.push_back(new_node);
			}
		}

		if (to_visit.size() > 0 && compare_vertixes != nullptr) { // second condition is when  we go in order of number of vertixes
			std::sort(to_visit.begin(), to_visit.end(), compare_vertixes);
		}

		for (std::size_t i = 0; i < to_visit.size(); i++) {
			if (already_visited[to_visit[i].end_vertex] == false) {
				depth_search_impl(to_visit[i].end_vertex, already_visited, process, process_after, compare_vertixes);
			}
		}
		if (process_after == true) {
			process(start_vertex);
		}
		to_visit.clear();
	}

	bool depth_search_impl_parent(int start_vertex, int parent_vertex, int* already_visited) {
		//process(start_vertex);
		already_visited[start_vertex] = 1;
		bool result = false;
		
		for (std::size_t i = 0; i < this->size; i++) {
			if (this->matrix[start_vertex][i] != 0) {
				if (already_visited[i] == 0) {
					result = depth_search_impl_parent(i, start_vertex, already_visited);
				}
				else if (already_visited[i] == 1 && i != parent_vertex && parent_vertex != -1 || start_vertex == i) {
					result = true;
				}

				if (result == true) { return true; }
			}
		}
		already_visited[start_vertex] = 2;
		return false;
	}


	template <typename Callable>
	void breadth_search_impl(std::size_t start_vertex, bool* already_visited, Callable process, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		Queue<std::size_t> to_visit;
		to_visit.enqueue(start_vertex);
		already_visited[start_vertex] = true;

		while (!to_visit.is_empty()) {
			std::size_t current_vertex = to_visit.dequeue();
			process(current_vertex);

			std::vector<GraphNode> neighbors;

			for (std::size_t j = 0; j < this->size; j++) { // put all neighbors of vertex in vector to sort according to compare_vertixes
				if (this->matrix[current_vertex][j] != 0) {
					GraphNode new_node(j, this->matrix[current_vertex][j]);
					neighbors.push_back(new_node);
				}
				
			}

			if (neighbors.size() != 0 && compare_vertixes != nullptr) {
				std::sort(neighbors.begin(), neighbors.end(), compare_vertixes);
			}

			for (std::size_t i = 0; i < neighbors.size(); i++) {
				if (already_visited[neighbors[i].end_vertex] == false) {
					to_visit.enqueue(neighbors[i].end_vertex);
					already_visited[neighbors[i].end_vertex] = true;
				}
			}
			neighbors.clear();
		}
	}

	void topological_sort_impl(std::size_t* vertex_order) {

	}


};


AdjMatrix generate_random_matrix(std::size_t size, std::size_t edge_number = 0, std::size_t mode = directed) {
	assert(size > 0);
	if (mode == directed) {
		assert(edge_number <= size * (size - 1));
	}
	else if (mode == undirected) {
		assert(edge_number <= (size * (size - 1))/2);
	}
	
	AdjMatrix new_matrix(size);
	{
		for (std::size_t i = 0; i < edge_number; i++) {
			std::size_t start_vertex;
			std::size_t end_vertex;
			int edge_weight;

			do {
				edge_weight = rand() % 20;
				start_vertex = rand() % size;
				end_vertex = rand() % size;
			} while (start_vertex == end_vertex || new_matrix.matrix[start_vertex][end_vertex] != 0 || edge_weight == 0);

			if (mode == directed) {
				new_matrix.add_edge(start_vertex, end_vertex, edge_weight);
			}
			else if (mode == undirected) {
				new_matrix.add_undirected_edge(start_vertex, end_vertex, edge_weight);
			}
			

		}

		return new_matrix;
	}
}



struct AdjStruct {
	GraphNode** vertex;
	std::size_t size;

	AdjStruct(std::size_t size){
		this->size = size;
		this->vertex = new GraphNode * [size];
		for (std::size_t i = 0; i < size; i++) {
			this->vertex[i] = nullptr;
		}
	}

	void add_edge(std::size_t start_vertex, std::size_t end_vertex, int weight) {
		assert(start_vertex < this->size);
		assert(end_vertex < this->size);
		assert(!is_edge(start_vertex, end_vertex) && "Try to replace existing edge");

		if (this->vertex[start_vertex] == nullptr || end_vertex < this->vertex[start_vertex]->end_vertex) {
			GraphNode* next = this->vertex[start_vertex];
			GraphNode* new_node = new GraphNode(end_vertex, weight, next);
			this->vertex[start_vertex] = new_node;
		}
		else if (end_vertex != this->vertex[start_vertex]->end_vertex){
			//GraphNode* previous = this->vertex[start_vertex];
			GraphNode* current = this->vertex[start_vertex];

			while (current->next && current->next->end_vertex < end_vertex) {
				current = current->next;
			}
			if (current->next && current->next->end_vertex == end_vertex) {
				return; // nothing to do
			}
			else {
				GraphNode* new_node = new GraphNode(end_vertex, weight, current->next);
				current->next = new_node;
			}
		}
	}

	void remove_edge(std::size_t start_vertex, std::size_t end_vertex) {
		assert(start_vertex < this->size);
		assert(end_vertex < this->size);
		assert(is_edge(start_vertex, end_vertex) && "Try to remove unexisting edge");

		GraphNode* current = this->vertex[start_vertex];
		if (!current) { return; }
		if (current->end_vertex == end_vertex) { // if this vertex is the first in adjacent list
			this->vertex[start_vertex] = current->next;
			delete current;
		}
		else {
			GraphNode* previous = current;
			current = current->next;
			while (current && current->end_vertex <= end_vertex) { // current->end_vertex < end_vertex (???) // end_vertex <= current->end_vertex
				if (current->end_vertex == end_vertex) {
					previous->next = current->next;
					delete current;
					break;
				}
				previous = previous->next;
				current = current->next;
			}
		}
	}
	
	void add_undirected_edge(std::size_t start, std::size_t end, int weight) {
		assert(start < this->size);
		assert(end < this->size);

		assert(!is_edge(start, end) && "Try to replace existing edge");
		assert(!is_edge(start, end) && "Try to replace existing edge");

		add_edge(start, end, weight);
		add_edge(end, start, weight);
	}

	void remove_undirected_edge(std::size_t start, std::size_t end) {
		assert(start < this->size);
		assert(end < this->size);

		assert(is_edge(start, end) != 0 && "Try to remove unexisting edge");
		assert(is_edge(start, end) != 0 && "Try to remove unexisting edge");

		
		GraphNode* current_1 = this->vertex[start];
		while (current_1 && current_1->end_vertex < end) {
			current_1 = current_1->next;
		}

		GraphNode* current_2 = this->vertex[end];
		while (current_2 && current_2->end_vertex < start) {
			current_2 = current_2->next;
		}

		assert(current_1->weight == current_2->weight && "Try to remove two different directed edges");

		remove_edge(start, end);
		remove_edge(end, start);
	}
	

	void print() {
		bool is_empty = true;
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			if (current) {
				is_empty = false;
				while (current) {
					std::cout << i << "->" << current->end_vertex << " (" << current->weight << "), ";
					current = current->next;
				}
				std::cout << std::endl;
			}
		}
		if (is_empty) {
			std::cout << "No edges" << std::endl;
		}
	}

	bool is_edge(std::size_t start_vertex, std::size_t end_vertex) {
		assert(start_vertex < this->size);
		assert(end_vertex < this->size);

		GraphNode* current = this->vertex[start_vertex];
		while (current && current->end_vertex <= end_vertex) {
			if (current->end_vertex == end_vertex) {
				return true;
			}
			current = current->next;
		}

		return false;
	}

	void transitive_closure() {
		for (std::size_t i = 0; i < this->size; ) {
			bool is_end = true;
			GraphNode* current = this->vertex[i];
			while (current) {
				GraphNode* j = this->vertex[current->end_vertex];
				while (j) {
					if (this->is_edge(i, j->end_vertex) == false && i != j->end_vertex) {
						add_edge(i, j->end_vertex, rand() % 9 + 1);
						is_end = false;
					}
					j = j->next;
				}
				current = current->next;
			}

			if (is_end == true) { // checking is we have added something new in this loop. If, then we are on the same vertex and process it
				i++; // else we go to the next vertex
			}
		}
	}

	bool is_connected() {
		for (std::size_t i = 0; i < this->size; i++) {
			std::size_t vertex_number = 0;
			depth_search_one_component(i, [&vertex_number](std::size_t vertex) {vertex_number++; });
			if (vertex_number < this->size) {
				return false;
			}
		}
		return true;
	}

	bool is_cycles() {
//		bool* already_visited = new bool[this->size];
//		for (std::size_t i = 0; i < this->size; i++) {
//			already_visited[i] = false;
//		}

		for (std::size_t i = 0; i < this->size; i++) {

			bool* already_visited = new bool[this->size];
			for (std::size_t j = 0; j < this->size; j++) {
				already_visited[j] = false;
			}

//			if (already_visited[i] == false) {
			if (depth_search_impl_parent(i, -1, already_visited) == true) {
				return true;
			}
//			}
			delete[]already_visited;
		}
//		delete[]already_visited;
		return false;
	}

	bool is_tree() {
		return !is_cycles() && is_connected();
	}


	template <typename Callable>
	void depth_search_all(Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		for (std::size_t i = 0; i < this->size; i++) {
			if (already_visited[i] == false) {
				depth_search_impl(i, already_visited, process, process_after, compare_vertixes);
			}
		}
		delete[]already_visited;
	}

	template <typename Callable>
	void depth_search_one_component(std::size_t start_vertex, Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
		assert(start_vertex < this->size);

		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}
		
		depth_search_impl(start_vertex, already_visited, process, process_after, compare_vertixes);
		delete[]already_visited;
	}


	template <typename Callable>
	void breadth_search_all(Callable process, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		for (std::size_t i = 0; i < this->size; i++) {
			if (already_visited[i] == false) {
				breadth_search_impl(i, already_visited, process, compare_vertixes);
			}
		}
		delete[]already_visited;
	}

	template <typename Callable>
	void breadth_search_one_component(std::size_t start_vertex, Callable process, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
		assert(start_vertex < this->size);

		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		breadth_search_impl(start_vertex, already_visited, process, compare_vertixes);
		delete[]already_visited;
	}


	std::vector<std::size_t> get_distance(std::size_t start_vertex, std::vector <int>& prev) {
		const std::size_t INF = INT_MAX;
		std::vector<std::size_t> distances(this->size, INF);
		distances[start_vertex] = 0;

		std::vector <bool> used(this->size, false);

		std::size_t min_dist = 0;
		std::size_t min_vertex = start_vertex;
		while (min_dist < INF) {
			//std::size_t i = min_vertex;
			used[min_vertex] = true;

			GraphNode* current = this->vertex[min_vertex];
			while (current) {
				if (distances[min_vertex] + current->weight < distances[current->end_vertex]) { // put marks to its neightbors
					distances[current->end_vertex] = distances[min_vertex] + current->weight;
					prev[current->end_vertex] = min_vertex;
				}
			}

			min_dist = INF;
			for (std::size_t j = 0; j < this->size; j++) { // choose the minimum distance
				if (!used[j] && distances[j] < min_dist) {
					min_dist = distances[j];
					min_vertex = j;
				}
			}
		}

		return distances;
	}

	std::vector<std::size_t> topological_sort() {
		// checking if graph has cycles in direct sense
		assert(!is_cycles() && "This graph can't be sorted");
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				assert(!is_edge(current->end_vertex, i) && "This graph can't be sorted");
				current = current->next;
			}
		}
		std::vector<std::size_t> vertices_order;
		depth_search_all([&vertices_order](std::size_t vertex) {vertices_order.push_back(vertex); }, true);
		std::reverse(vertices_order.begin(), vertices_order.end());
		return vertices_order;
		AdjStruct sorted_graph(this->size);
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				sorted_graph.add_edge(vertices_order[i], vertices_order[current->end_vertex], current->weight);
				current = current->next;
			}
		}
		//return sorted_graph;
	}

	private:

		template <typename Callable>
		void depth_search_impl(std::size_t start_vertex, bool* already_visited, Callable process, bool process_after = false, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
			if (process_after == false) {
				process(start_vertex);
			}
		
			already_visited[start_vertex] = true;

			GraphNode* current = this->vertex[start_vertex];
			std::vector<GraphNode*> to_visit;
			while (current) {
				to_visit.push_back(current);
				current = current->next;
			}

			if (to_visit.size() > 0 && compare_vertixes != nullptr) { // second condition is when  we go in order of number of vertixes
				std::sort(to_visit.begin(), to_visit.end(), compare_vertixes);
			}
			
			for (std::size_t i = 0; i < to_visit.size(); i++) {
				if (already_visited[to_visit[i]->end_vertex] == false) {
					depth_search_impl(to_visit[i]->end_vertex, already_visited, process, process_after, compare_vertixes);
				}
			}
			if (process_after == true) {
				process(start_vertex);
			}
			to_visit.clear();
		}

		bool depth_search_impl_parent(int start_vertex, int parent_vertex, bool* already_visited) {
			//process(start_vertex);
			already_visited[start_vertex] = true;
			bool result = false;
			GraphNode* current = this->vertex[start_vertex];
			while (current) {
				if (already_visited[current->end_vertex] == false) {
					result = depth_search_impl_parent(current->end_vertex, start_vertex, already_visited);
				}
				else if (current->end_vertex != parent_vertex && parent_vertex != -1) {
					result = true;
				}

				if (result == true) { return true; }
				current = current->next;
			}
			return false;
		}

		template <typename Callable>
		void breadth_search_impl(std::size_t start_vertex, bool* already_visited, Callable process, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
			Queue<std::size_t> to_visit;
			to_visit.enqueue(start_vertex);
			already_visited[start_vertex] = true;

			while (!to_visit.is_empty()) {
				std::size_t current_vertex = to_visit.dequeue();
				process(current_vertex);

				std::vector<GraphNode*> neighbors;
				GraphNode* current = this->vertex[current_vertex];
				while(current) { // put all neighbors of vertex in vector to sort according to compare_vertixes
					neighbors.push_back(current);
					current = current->next;
				}
				if (neighbors.size() != 0 && compare_vertixes != nullptr) {
					std::sort(neighbors.begin(), neighbors.end(), compare_vertixes);
				}

				for (std::size_t i = 0; i < neighbors.size(); i++) {
					if (already_visited[neighbors[i]->end_vertex] == false) {
						to_visit.enqueue(neighbors[i]->end_vertex);
						already_visited[neighbors[i]->end_vertex] = true;
					}
				}
				neighbors.clear();
			}
		}

};

AdjMatrix convert_in_matrix(AdjStruct& adj_struct) {
	AdjMatrix new_matrix(adj_struct.size);
	for (std::size_t i = 0; i < adj_struct.size; i++) {
		GraphNode* current = adj_struct.vertex[i];
		for (std::size_t j = 0; j < adj_struct.size; j++) {
			if (current) {
				if (current->end_vertex == j) {
					new_matrix.matrix[i][j] = current->weight;
					current = current->next;
				}
			}
			else { break; }
		}
	}
	return new_matrix;
}

AdjStruct convert_in_struct(AdjMatrix& adj_matrix) {
	AdjStruct new_struct(adj_matrix.size);
	for (std::size_t i = 0; i < adj_matrix.size; i++) {
		for (std::size_t j = 0; j < adj_matrix.size; j++) {
			if (adj_matrix.matrix[i][j] != 0) {
				new_struct.add_edge(i, j, adj_matrix.matrix[i][j]);
			}
		}
	}
	return new_struct;
}


AdjStruct generate_random_structure(std::size_t size, std::size_t edge_number, std::size_t mode = directed) {
	assert(size > 0);
	if (mode == directed) {
		assert(edge_number <= size * (size - 1));
	}
	else if (mode == undirected) {
		assert(edge_number <= (size * (size - 1)) / 2);
	}

	AdjStruct new_struct(size);
	for (std::size_t i = 0; i < edge_number; i++) {
		std::size_t start_vertex;
		std::size_t end_vertex;
		int edge_weight;

		do {
			edge_weight = rand() % 20;
			start_vertex = rand() % size;
			end_vertex = rand() % size;
		} while (start_vertex == end_vertex || new_struct.is_edge(start_vertex, end_vertex) == true || edge_weight == 0);


		if (mode == directed) {
			new_struct.add_edge(start_vertex, end_vertex, edge_weight);
		}
		else if (mode == undirected) {
			new_struct.add_undirected_edge(start_vertex, end_vertex, edge_weight);
		}

		//new_struct.add_edge(start_vertex, end_vertex, edge_weight);
	}
	return new_struct;
}

void process_print(std::size_t vertex) {
	std::cout << vertex << " ";
}

void process_nothing(std::size_t vertex) {}


bool compare_weight_struct(GraphNode* node_1, GraphNode* node_2) {
	return node_1->weight < node_2->weight;
}

bool compare_number_struct(GraphNode* node_1, GraphNode* node_2) {
	return node_1->end_vertex < node_2->end_vertex;
}

bool compare_weight_matrix(GraphNode& node_1, GraphNode& node_2) {
	return node_1.weight < node_2.weight;
}

bool compare_number_matrix(GraphNode& node_1, GraphNode& node_2) {
	return node_1.end_vertex < node_2.end_vertex;
}

template <typename T>
std::vector<Path> get_path_from_one(T& graph, std::size_t vertex) {
	std::vector <int> prev(graph.size, -1);
	std::vector<std::size_t> distances = graph.get_distance(vertex, prev);
	std::vector <Path> paths_struture;

	for (std::size_t i = 0; i < distances.size(); i++) {
		Path a;
		int j = i;
		if (prev[j] != -1) {
			a.is_exist = true;
			while (j != -1) { // put the path
				a.path.push_back(j);
				j = prev[j];
			}
		}
		else {
			a.is_exist = false;
		}
		reverse(a.path.begin(), a.path.end());

		a.lenght = distances[i]; // put the length

		paths_struture.push_back(a);
	}
	return paths_struture;
}

template <typename T>
Path get_path_between_two(T& graph, std::size_t start_vertex, std::size_t end_vertex) {
	std::vector <int> prev(graph.size, -1);
	std::vector<std::size_t> distances = graph.get_distance(start_vertex, prev);

	Path a;
	int j = start_vertex;
	if (prev[j] != -1) {
		a.is_exist = true;
		while (j != -1) { // put the path
			a.path.push_back(j);
			j = prev[j];
		}
	}
	else {
		a.is_exist = false;
	}
	reverse(a.path.begin(), a.path.end());

	a.lenght = distances[start_vertex]; // put the length

	return a;
}

template <typename T>
Path** get_path_from_all(T& graph) {
	Path** path_matrix = new Path * [graph.size];
	for (std::size_t i = 0; i < graph.size; i++) {
		path_matrix[i] = new Path[graph.size];
	}

	for (std::size_t k = 0; k < graph.size; k++) { // path from k to i;
		std::vector <int> prev(graph.size, -1);
		std::vector<std::size_t> distances = graph.get_distance(k, prev);

		for (std::size_t i = 0; i < distances.size(); i++) {
			Path a;
			int j = i;
			if (prev[j] != -1) {
				a.is_exist = true;
				while (j != -1) { // put the path
					a.path.push_back(j);
					j = prev[j];
				}
			}
			else {
				a.is_exist = false;
			}
			reverse(a.path.begin(), a.path.end());

			a.lenght = distances[i]; // put the length

			path_matrix[k][i] = a;
		}
	}
	return path_matrix;
}




void print_path(Path path, std::size_t start_vertex, std::size_t end_vertex) { // print path from start_vertex to end_vertex

	std::cout << start_vertex << "->" << end_vertex << ": ";
	if (!path.is_exist) {
		std::cout << "does not exist";
	}
	else {
		for (std::size_t j = 0; j < path.path.size(); j++) {
			std::cout << path.path[j] << " ";
		}
		std::cout << "  =  " << path.lenght;
	}
	std::cout << std::endl;

}

void print_path_structure(std::vector<Path>& path_structure, std::size_t start_vertex) { // print path from 1 to all
	for (std::size_t i = 0; i < path_structure.size(); i++) {
		print_path(path_structure[i], start_vertex, i);
	}
}

void print_path_all(Path** path_matrix, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			print_path(path_matrix[i][j], i, j);
		}
		std::cout << std::endl;
	}
}


int main() {
	AdjMatrix graph1(5);
	//graph1.add_edge(1, 2, 5);
	graph1.add_edge(1, 3, 5);
	graph1.print_matrix();
	graph1.print_edges();
	std::cout << "\nRemove edge 1->3:\n";
	graph1.remove_edge(1, 3);
	graph1.print_matrix();
	graph1.print_edges();
	std::cout << "\nGenerate random graph:\n";
	AdjMatrix graph2 = generate_random_matrix(6, 5);
	graph2.print_matrix();
	graph2.print_edges();
	std::cout << "\nAdj struct:\n";
	AdjStruct graph3(5);
	graph3.add_edge(1, 4, 6);
//	graph3.add_edge(1, 2, 6);
//	graph3.add_edge(1, 1, 6);
//	graph3.add_edge(1, 2, 6);
//	graph3.add_edge(1, 2, 6);
//	graph3.add_edge(1, 2, 6);
	graph3.add_edge(1, 3, 6);
//	graph3.add_edge(1, 2, 6);
	graph3.add_edge(1, 2, 6);
//	graph3.add_edge(2, 1, 6);
	graph3.print();

	std::cout << "\nRemove edge 1->2\n";
//	graph3.remove_edge(1, 0);
//	graph3.remove_edge(2, 1);
//	graph3.remove_edge(3, 1);
///	graph3.remove_edge(1, 2);
//	graph3.remove_edge(1, 4);
//	graph3.remove_edge(1, 4);
	graph3.print();

	std::cout << "\nConvert from Adj struct to matrix:\n";
	std::cout << "Adj struct:\n";
	AdjStruct graph4(5);
	graph4.add_edge(0, 4, 6);
	graph4.add_edge(0, 2, 7);
	graph4.add_edge(3, 0, 8);
	graph4.add_edge(2, 1, 9);
	graph4.add_edge(4, 3, 3);
	graph4.add_edge(2, 0, 5);

//	graph4.add_edge(0, 2, 8);

	graph4.print();
	std::cout << "\nCopied struct:\n";
	AdjMatrix graph4_copy = convert_in_matrix(graph4);
	graph4_copy.print_edges();
	std::cout << std::endl;
	graph4_copy.print_matrix();


	std::cout << "\nConvert from Adj matrix to struct:\n";
	std::cout << "Adj matrix:\n";
	AdjMatrix graph5(5);
	graph5.add_edge(0, 4, 6);
	graph5.add_edge(0, 2, 7);
	graph5.add_edge(3, 0, 8);
	graph5.add_edge(2, 1, 9);
	graph5.add_edge(4, 3, 3);
	graph5.add_edge(2, 0, 5);

//	graph5.add_edge(0, 2, 8);

	graph5.print_matrix();
	graph5.print_edges();
	std::cout << "\nCopied matrix:\n";
	AdjStruct graph5_copy = convert_in_struct(graph5);
	graph5_copy.print();

	std::cout << "\nGenerate random AdjStruct:\n";
	AdjStruct graph6 = generate_random_structure(5, 6);
	graph6.print();
	
	std::cout <<"\nTransitive closure of AdjMatrix:\n";
	AdjMatrix graph7(4);
	graph7.add_edge(0, 1, 1);
	graph7.add_edge(1, 2, 1);
	graph7.add_edge(2, 3, 1);
	//graph7.add_edge(3, 0, 5);
	graph7.print_matrix();
	std::cout << "\nAfter transitive closure of AdjMatrix:\n";
	graph7.transitive_closure();
	graph7.print_matrix();

	std::cout << "\nTransitive closure of AdjStruct:\n";
	AdjStruct graph8(4);
	graph8.add_edge(0, 1, 1);
	graph8.add_edge(1, 2, 1);
	graph8.add_edge(2, 3, 1);
	//graph8.add_edge(0, 2, 1);
	graph8.print();
	//graph7.add_edge(3, 0, 5);
	convert_in_matrix(graph8).print_matrix();
	std::cout << "\nAfter transitive closure of AdjStruct:\n";
	graph8.transitive_closure();
	graph8.print();
	convert_in_matrix(graph8).print_matrix();




	std::cout << "\n\n\n\n====================SEARCH===========\n\n\n";
	std::cout << "\nDepth search in AdjStruct from vertex 0:\n";
	AdjStruct graph9(7);
	graph9.add_edge(0, 1, 1);
	graph9.add_edge(0, 4, 1);
	graph9.add_edge(4, 0, 1);
	graph9.add_edge(2, 1, 1);
	graph9.add_edge(2, 3, 1);
	graph9.add_edge(2, 5, 1);
	graph9.add_edge(6, 2, 1);
	graph9.print();
	std::cout << std::endl;
	AdjMatrix graph10(7);
	graph10.add_edge(0, 1, 1);
	graph10.add_edge(0, 4, 1);
	graph10.add_edge(4, 0, 8);
	graph10.add_edge(2, 1, 5);
	graph10.add_edge(2, 3, 1);
	graph10.add_edge(2, 5, 12);
	graph10.add_edge(6, 2, 18);
	graph10.print_edges();
	graph10.print_matrix();

	std::size_t I = 6;

	std::cout << "\nDepth search in AdjStruct from vertex " << I << ": \n";
	graph9.depth_search_one_component(I, process_print, compare_number_struct);

	std::cout << "\nDepth search in AdjMatrix from vertex " << I << ": \n";
	graph10.depth_search_one_component(I, process_print, compare_number_matrix);


	std::cout << "\nBreadth search in AdjStruct from vertex " << I << ": \n";
	graph9.breadth_search_one_component(I, process_print, compare_number_struct);

	std::cout << "\nBreadth search in AdjMatrix from vertex " << I << ": \n";
	graph10.breadth_search_one_component(I, process_print, compare_weight_matrix);


	std::cout << "\n\nAdjStruct:\n";

	if (graph9.is_connected()) {
		std::cout << "\nConnected: YES\n";
	}
	else {
		std::cout << "\nConnected: NO\n";
	}

	if (graph9.is_cycles()) {
		std::cout << "Cycles: YES\n";
	}
	else {
		std::cout << "Cycles: NO\n";
	}

	if (graph9.is_tree()) {
		std::cout << "Tree: YES\n";
	}
	else {
		std::cout << "Tree: NO\n";
	}
	std::cout << std::endl;


	std::cout << "\n\nAdjMatrix:";

	if (graph10.is_connected()) {
		std::cout << "\nConnected: YES\n";
	}
	else {
		std::cout << "\nConnected: NO\n";
	}

	if (graph10.is_cycles()) {
		std::cout << "Cycles: YES\n";
	}
	else {
		std::cout << "Cycles: NO\n";
	}

	if (graph10.is_tree()) {
		std::cout << "Tree: YES\n";
	}
	else {
		std::cout << "Tree: NO\n";
	}
	std::cout << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "\n\nUndirected edges:\n\n";
	AdjMatrix graph11(6);
	graph11.add_undirected_edge(4, 5, 3);
	graph11.add_undirected_edge(1, 5, 6);
	graph11.add_undirected_edge(4, 3, 9);
	graph11.add_undirected_edge(2, 1, 1);
	graph11.add_undirected_edge(0, 2, 7);
	graph11.print_edges();
	std::cout << std::endl << std::endl;
//	graph11.remove_undirected_edge(5, 0);
	graph11.print_edges();
	

	std::cout << "\n\nUndirected edges (generate):\n\n";
	generate_random_matrix(6, 7, undirected).print_matrix();
	std::cout << std::endl;
	generate_random_structure(6, 7, undirected).print();



	std::size_t J = 6, SIZE  = 7;
	AdjMatrix graph12(SIZE);
	graph12.add_edge(0, 1, 1);
	graph12.add_edge(0, 4, 1);
	graph12.add_edge(4, 0, 8);
	graph12.add_edge(2, 1, 5);
	graph12.add_edge(2, 3, 100); // 
	graph12.add_edge(2, 5, 12);
	graph12.add_edge(6, 2, 18);
	graph12.add_edge(5, 3, 10);
	graph12.add_edge(1, 3, 21);
	graph12.print_edges();
	graph12.print_matrix();

	AdjMatrix graph13(SIZE);
	graph13.add_edge(0, 1, 1);
	graph13.add_edge(0, 4, 1);
	graph13.add_edge(4, 0, 8);
	graph13.add_edge(2, 1, 5);
	graph13.add_edge(2, 3, 100); // 
	graph13.add_edge(2, 5, 12);
	graph13.add_edge(6, 2, 18);
	graph13.add_edge(5, 3, 10);
	graph13.add_edge(1, 3, 21);
	graph13.print_edges();
	graph13.print_matrix();


	for (std::size_t J = 0; J < graph12.size; J++) {
		std::cout << "\n\n\n\n\nMIN distances from vertex " << J << ": \n\n\n";
		std::vector<Path> path_structure = get_path_from_one(graph12,J);

		print_path_structure(path_structure, J);
	}
	std::cout << "\n\n\n==============\n";
	print_path_all(get_path_from_all(graph13), graph13.size);


	std::cout << "\n\n\n===========Topological sort====================\n";
	AdjMatrix graph14(6);
	/*
	graph14.add_edge(5, 0, 1);
	graph14.add_edge(4, 0, 1);
	graph14.add_edge(5, 2, 1);
	graph14.add_edge(4, 1, 1);
	graph14.add_edge(2, 3, 1);
	graph14.add_edge(3, 1, 1);
	*/
	graph14.add_edge(1, 2, 1);
	graph14.add_edge(1, 3, 1);
	graph14.add_edge(3, 2, 1);
	graph14.add_edge(2, 4, 1);
	graph14.add_edge(3, 4, 1);


	graph14.print_matrix();
	graph14.print_edges();
	std::cout << "\n\nSorted vertices:\n";
	std::vector<std::size_t> arr = graph14.topological_sort();
	for (std::size_t i = 0; i < arr.size(); i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
//	graph14.topological_sort().topological_sort().print_matrix();
	std::system("pause");
	return 0;
}