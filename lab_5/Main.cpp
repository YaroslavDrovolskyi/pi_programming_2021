// 1, 2, 5, 7, 8, 10, 11, 13, 14

#include "queue.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <climits>

enum generate_mode{directed, undirected};

void process_nothing(std::size_t vertex) {}
template <typename T>
std::size_t components_number(T& graph);

struct Edge {
	std::size_t start_vertex;
	std::size_t end_vertex;
	int weight;

	Edge(std::size_t start_vertex, std::size_t end_vertex, int weight) {
		this->start_vertex = start_vertex;
		this->end_vertex = end_vertex;
		this->weight = weight;
	}

	Edge() {
		this->start_vertex = 0;
		this->end_vertex = 0;
		this->weight = 0;
	}
};

bool compare_edges(Edge& edge_1, Edge& edge_2) {
	return edge_1.weight > edge_2.weight;
}

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

template <typename T>
int get_index(std::vector<T>& arr, T key) {
	for (std::size_t i = 0; i < arr.size(); i++) {
		if (arr[i] == key) { return i; }
	}
	return -1; // when no key in array
}


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
					if (this->matrix[i][j] == this->matrix[j][i] && i <= j) {
						std::cout << i << "<->" << j << " (" << this->matrix[i][j] << "), ";
						is_edge = true;
						is_empty = false;
					}
					else if (this->matrix[i][j] == this->matrix[j][i] && i > j) {
						; // we are on undirected edge, that we've printed before
					}
					else {
						std::cout << i << "->" << j << " (" << this->matrix[i][j] << "), ";
						is_edge = true;
						is_empty = false;
					}
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
					if (this->matrix[i][k] && this->matrix[k][j] && this->matrix[i][j] == 0) {
						if (i != j) {
							this->matrix[i][j] = rand() % 9 + 1;
						}
					}
				}
			}
		}
	}
	
	
	void clean() {
		for (std::size_t i = 0; i < this->size; i++) {
			delete[]this->matrix[i];
		}
		delete []this->matrix;
		this->size = 0;
	}

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

	AdjMatrix topological_sort() {
//	std::vector<std::size_t> topological_sort() {
		// checking if graph has cycles in direct sense
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = i; j < this->size; j++) {
				if (this->matrix[i][j] != 0) {
					assert(this->matrix[j][i] == 0 && "This graph can't be sorted: undirected edge exists");
				}
			}
		}
		assert(!is_cycles() && "This graph can't be sorted: cycle exists");

		std::vector<std::size_t> vertices_order;
		depth_search_all([&vertices_order](std::size_t vertex) {vertices_order.push_back(vertex); }, true);
		std::reverse(vertices_order.begin(), vertices_order.end());
//		return vertices_order;

		//create sorted graph
		AdjMatrix sorted_graph(this->size);
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = 0; j < this->size; j++) {
				sorted_graph.matrix[get_index(vertices_order, i)][get_index(vertices_order, j)] = this->matrix[i][j];
			}
		}
		return sorted_graph;
	}

	/*
	AdjMatrix spanning_tree(std::size_t& spanning_weight, bool (*compare_vertices)(GraphNode&, GraphNode&) = nullptr) {
		AdjMatrix spanning_graph (this->size);
		spanning_weight = 0; // re-initialize, if it was not 0

		bool* already_visited = new bool[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			already_visited[i] = false;
		}

		for (std::size_t i = 0; i < this->size; i++) { // to build spanning forest, if graph isn't connected
			if (already_visited[i] == false) {
				spanning_tree_impl(i, already_visited, spanning_graph, spanning_weight, compare_vertices);
			}
		}
		delete[]already_visited;
		return spanning_graph;
	}
	*/
	
	//build spanning tree only for undirected graphs
	void spanning_tree_impl(std::size_t start_vertex, bool* already_visited, AdjMatrix& spanning_graph, std::size_t& spanning_weight, bool (*compare_vertixes)(GraphNode&, GraphNode&) = nullptr) {
		Queue<std::size_t> to_visit;
		to_visit.enqueue(start_vertex);
		already_visited[start_vertex] = true;
//		int parent = -1;

		while (!to_visit.is_empty()) {
			std::size_t current_vertex = to_visit.dequeue();

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

					if (this->matrix[current_vertex][neighbors[i].end_vertex] != 0 && spanning_graph.matrix[current_vertex][neighbors[i].end_vertex] == 0) {
						spanning_graph.add_edge(current_vertex, neighbors[i].end_vertex, this->matrix[current_vertex][neighbors[i].end_vertex]);
					}
					
					if (this->matrix[neighbors[i].end_vertex][current_vertex] != 0 && spanning_graph.matrix[neighbors[i].end_vertex][current_vertex] == 0) {
						spanning_graph.add_edge(neighbors[i].end_vertex, current_vertex, this->matrix[neighbors[i].end_vertex][current_vertex]);
					}
					spanning_weight += this->matrix[current_vertex][neighbors[i].end_vertex];
				}
			}
			neighbors.clear();
			//parent = current_vertex;
		}
	}

	bool is_undirected() {
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = i; j < this->size; j++) {
				if (this->matrix[i][j] != this->matrix[j][i]) {
					return false;
				}
			}
		}
		return true;
	}

	std::vector<Edge> capture_edges(std::size_t& weight) {
		std::vector<Edge> edges;
		weight = 0; // re-initialize, if it was 0
		for (std::size_t i = 0; i < this->size; i++) {
			for (std::size_t j = i; j < this->size; j++) {
				if (this->matrix[i][j] != 0) {
					Edge new_edge(i, j, this->matrix[i][j]);
					edges.push_back(new_edge);
					weight += this->matrix[i][j];
				}
			}
		}
		return edges;
	}


	

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

	private:
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
		assert(!is_edge(end, start) && "Try to replace existing edge");

		add_edge(start, end, weight);
		add_edge(end, start, weight);
	}

	void remove_undirected_edge(std::size_t start, std::size_t end) {
		assert(start < this->size);
		assert(end < this->size);

		assert(is_edge(start, end) == true && "Try to remove unexisting edge");
		assert(is_edge(end, start) == true && "Try to remove unexisting edge");

		
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
	

	void print_edges() {
		bool is_empty = true;
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			if (current) {
				is_empty = false;
				bool is_newline = false;
				while (current) {
					if (is_edge(current->end_vertex, i) && i < current->end_vertex) { // if it is undirected edge at the 1st time
						std::cout << i << "<->" << current->end_vertex << " (" << current->weight << "), ";
						is_newline = true;
					}
					else if (!is_edge(current->end_vertex, i)) { // if it is directed edge
						std::cout << i << "->" << current->end_vertex << " (" << current->weight << "), ";
						is_newline = true;
					}
					current = current->next;
				}
				if (is_newline) {
					std::cout << std::endl;
				}
				
			}
		}
		if (is_empty) {
			std::cout << "No edges" << std::endl;
		}
	}
	void print_matrix() {
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			for (std::size_t j = 0; j < size; j++) {
				if (current && current->end_vertex == j) {
					std::cout << current->weight << " ";
					current = current->next;
				}
				else {
					std::cout << "0 ";
				}
			}
			std::cout << std::endl;
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

	GraphNode* get_edge(std::size_t start_vertex, std::size_t end_vertex) {
		assert(start_vertex < this->size);
		assert(end_vertex < this->size);

		GraphNode* current = this->vertex[start_vertex];
		while (current && current->end_vertex <= end_vertex) {
			if (current->end_vertex == end_vertex) {
				return current;
			}
			current = current->next;
		}

		return nullptr;
	}

	void clean() {
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* head = this->vertex[i];
			GraphNode* current = head;
			while (head) {
				head = head->next;
				delete current;
				current = head;
			}
		}
		delete[]this->vertex;
		this->size = 0;
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
				current = current->next;
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

	AdjStruct topological_sort() {
//	std::vector<std::size_t> topological_sort() {
		// checking if graph has cycles in direct sense
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				assert(!is_edge(current->end_vertex, i) && "This graph can't be sorted: undirected edge exists");
				current = current->next;
			}
		}
		assert(!is_cycles() && "This graph can't be sorted: cycle exists");

		std::vector<std::size_t> vertices_order;
		depth_search_all([&vertices_order](std::size_t vertex) {vertices_order.push_back(vertex); }, true);
		std::reverse(vertices_order.begin(), vertices_order.end());
//		return vertices_order;

		// create sorted graph
		AdjStruct sorted_graph(this->size);
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				sorted_graph.add_edge(get_index(vertices_order, i), get_index(vertices_order, current->end_vertex), current->weight);
				current = current->next;
			}
		}
		return sorted_graph;
	}

	void spanning_tree_impl(std::size_t start_vertex, bool* already_visited, AdjStruct& spanning_graph, std::size_t& spanning_weight, bool (*compare_vertixes)(GraphNode*, GraphNode*) = nullptr) {
		Queue<std::size_t> to_visit;
		to_visit.enqueue(start_vertex);
		already_visited[start_vertex] = true;

		while (!to_visit.is_empty()) {
			std::size_t current_vertex = to_visit.dequeue();

			std::vector<GraphNode*> neighbors;

			GraphNode* current = this->vertex[current_vertex];
			while (current) { // put all neighbors of vertex in vector to sort according to compare_vertixes
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
					if (!spanning_graph.is_edge(current_vertex, neighbors[i]->end_vertex)) {
						spanning_graph.add_edge(current_vertex, neighbors[i]->end_vertex, neighbors[i]->weight);
					}
					
					if (is_edge(neighbors[i]->end_vertex, current_vertex) && !spanning_graph.is_edge(neighbors[i]->end_vertex, current_vertex)) {
						spanning_graph.add_edge(neighbors[i]->end_vertex, current_vertex, neighbors[i]->weight);
					}
					spanning_weight += neighbors[i]->weight;
				}
			}
			neighbors.clear();
		}
	}

	bool is_undirected() {
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				if (!is_edge(current->end_vertex, i) || get_edge(current->end_vertex, i)->weight != current->weight) { return false; } // che�k if reverse edge exists
				current = current->next;
			}
		}
		return true;
	}

	std::vector<Edge> capture_edges(std::size_t& weight) {
		std::vector<Edge> edges;
		weight = 0; // if it wasn't 0
		for (std::size_t i = 0; i < this->size; i++) {
			GraphNode* current = this->vertex[i];
			while (current) {
				if (i <= current->end_vertex) {
					Edge new_edge(i, current->end_vertex, current->weight);
					edges.push_back(new_edge);
					weight += current->weight;
				}
				current = current->next;
			}
		}
		return edges;
	}
	
//	private:

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

		bool depth_search_impl_parent(int start_vertex, int parent_vertex, int* already_visited) {
			//process(start_vertex);
			already_visited[start_vertex] = 1;
			bool result = false;

			GraphNode* current = this->vertex[start_vertex];
			while (current) {
				if (already_visited[current->end_vertex] == 0) {
					result = depth_search_impl_parent(current->end_vertex, start_vertex, already_visited);
				}
				else if (already_visited[current->end_vertex] == 1 && current->end_vertex != parent_vertex && parent_vertex != -1 || start_vertex == current->end_vertex) { // last condition is to catch a loop
					result = true;
				}

				if (result == true) { return true; }
				current = current->next;
			}
			already_visited[start_vertex] = 2;
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
	assert(vertex < graph.size);
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
	assert(start_vertex < graph.size);
	assert(end_vertex < graph.size);
	std::vector <int> prev(graph.size, -1);
	std::vector<std::size_t> distances = graph.get_distance(start_vertex, prev);

	Path a;
	int j = end_vertex;
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

	a.lenght = distances[end_vertex]; // put the length

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

template <typename T, typename Callable>
T spanning_tree(T& graph, std::size_t& spanning_weight, Callable compare_vertices = nullptr) {
	assert(graph.is_undirected() == true && "Can't build spanning tree / forest in directed graph");
	T spanning_graph(graph.size);
	spanning_weight = 0; // re-initialize, if it was not 0

	bool* already_visited = new bool[graph.size];
	for (std::size_t i = 0; i < graph.size; i++) {
		already_visited[i] = false;
	}

	for (std::size_t i = 0; i < graph.size; i++) { // this loop is to build spanning forest, if graph isn't connected
		if (already_visited[i] == false) {
			graph.spanning_tree_impl(i, already_visited, spanning_graph, spanning_weight, compare_vertices);
		}
	}
	delete[]already_visited;
	return spanning_graph;
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

template <typename T>
std::size_t components_number(T& graph) {
	bool* already_visited = new bool[graph.size];
	for (std::size_t i = 0; i < graph.size; i++) {
		already_visited[i] = false;
	}
	std::size_t number = 0;
	for (std::size_t i = 0; i < graph.size; i++) {
		if (already_visited[i] == false) {
			number++;
			graph.depth_search_impl(i, already_visited, process_nothing);
		}
	}
	delete[]already_visited;
	return number;
}

template<typename T>
T min_spanning_tree(T& graph, std::size_t& min_weight) {
	assert(graph.is_undirected() && "Impossible to create MST: graph is directed");
	std::vector<Edge> edges = graph.capture_edges(min_weight); // add_edges
	
	std::sort(edges.begin(), edges.end(), compare_edges);
	T min_tree = graph;
	std::size_t comp_number = components_number(graph);
	for (std::size_t i = 0; i < edges.size(); i++) {
		min_tree.remove_undirected_edge(edges[i].start_vertex, edges[i].end_vertex);
		min_weight -= edges[i].weight;
		if (comp_number != components_number(min_tree)) { // if number of components increase
			min_tree.add_undirected_edge(edges[i].start_vertex, edges[i].end_vertex, edges[i].weight);
			min_weight += edges[i].weight;
		}
	}
	return min_tree;
}


template <typename T>
std::vector<std::vector<std::size_t>> connected_components(T& graph) { // for directed graphs result can be incorrect: all are depend on from what vertex we start
	std::vector<std::vector<std::size_t>> component_structure;

	bool* already_visited = new bool[graph.size];
	for (std::size_t i = 0; i < graph.size; i++) {
		already_visited[i] = false;
	}

	for (std::size_t i = 0; i < graph.size; i++) {
		if (already_visited[i] == false) {
			std::vector<std::size_t> single_component;
			graph.depth_search_impl(i, already_visited, [&single_component](std::size_t vertex) {single_component.push_back(vertex); });

			if (single_component.size() != 0) {
				component_structure.push_back(single_component);
			}
		}
	}
	delete[]already_visited;
	return component_structure;
}

void print_connected_components(std::vector<std::vector<std::size_t>>& component_structure) {
	for (std::size_t i = 0; i < component_structure.size(); i++) {
		std::cout << i << ": ";
		for (std::size_t j = 0; j < component_structure[i].size(); j++) {
			std::cout << component_structure[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

template<typename T>
void clean_matrix(T** matrix, std::size_t size);

void interactive();
void demonstrative();

int main() {
	short int next;
	std::cout << "Hello!\n\n\n";
	std::cout << "Choose program mode: 0 - exit, 1 - interactive, 2 - demonstrative, 3 - benchamrk\n\n";
	std::cin >> next;
	if (next == 1) {
		interactive();
	}
	else if(next == 2) {
		demonstrative();
	}
	else if (next == 3) {

	}
	else if (next == 4) {
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
		graph3.print_edges();

		std::cout << "\nRemove edge 1->2\n";
		//	graph3.remove_edge(1, 0);
		//	graph3.remove_edge(2, 1);
		//	graph3.remove_edge(3, 1);
		///	graph3.remove_edge(1, 2);
		//	graph3.remove_edge(1, 4);
		//	graph3.remove_edge(1, 4);
		graph3.print_edges();

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

		graph4.print_edges();
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
		graph5_copy.print_edges();

		std::cout << "\nGenerate random AdjStruct:\n";
		AdjStruct graph6 = generate_random_structure(5, 6);
		graph6.print_edges();

		std::cout << "\nTransitive closure of AdjMatrix:\n";
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
		graph8.print_edges();
		//graph7.add_edge(3, 0, 5);
		convert_in_matrix(graph8).print_matrix();
		std::cout << "\nAfter transitive closure of AdjStruct:\n";
		graph8.transitive_closure();
		graph8.print_edges();
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
		graph9.print_edges();
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
		generate_random_structure(6, 7, undirected).print_edges();



		std::size_t J = 6, SIZE = 7;
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
			std::vector<Path> path_structure = get_path_from_one(graph12, J);

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
		graph14.add_edge(0, 3, 1);
		graph14.add_edge(0, 4, 1);
		graph14.add_edge(0, 5, 1);
		graph14.add_edge(3, 4, 1);
		graph14.add_edge(4, 5, 1);
		graph14.add_edge(3, 2, 1);
		graph14.add_edge(2, 1, 1);
		graph14.add_edge(4, 1, 1);
		graph14.add_edge(5, 1, 1);
		graph14.add_edge(4, 2, 1);
		//	graph14.add_edge(1, 4, 1);


		graph14.print_matrix();
		graph14.print_edges();
		std::cout << std::endl;

		AdjStruct graph15(6);
		graph15.add_edge(0, 3, 1);
		graph15.add_edge(0, 4, 1);
		graph15.add_edge(0, 5, 1);
		graph15.add_edge(3, 4, 1);
		graph15.add_edge(4, 5, 1);
		graph15.add_edge(3, 2, 1);
		graph15.add_edge(2, 1, 1);
		graph15.add_edge(4, 1, 1);
		graph15.add_edge(5, 1, 1);
		graph15.add_edge(4, 2, 1);
		graph15.print_edges();


		std::cout << "\n\nSorted graph14:\n";
		AdjMatrix arr14_sorted = graph14.topological_sort();
		arr14_sorted.print_matrix();
		std::cout << std::endl;
		//	std::vector<std::size_t> arr14 = graph14.topological_sort();
		//	for (std::size_t i = 0; i < arr14.size(); i++) {
		//		std::cout << arr14[i] << " ";
		//	}
		//	std::cout << std::endl;

		std::cout << "\n\nSorted graph15:\n";
		//	std::vector<std::size_t> arr15 = graph15.topological_sort();
		//	for (std::size_t i = 0; i < arr15.size(); i++) {
		//		std::cout << arr15[i] << " ";
		//	}
		AdjStruct arr15_sorted = graph15.topological_sort();
		convert_in_matrix(arr15_sorted).print_matrix();
		std::cout << std::endl;

		AdjStruct graph16(6);
		graph16.add_edge(0, 1, 1);
		graph16.add_edge(2, 1, 1);
		graph16.add_edge(3, 1, 1);
		graph16.add_edge(1, 4, 1);
		graph16.add_edge(5, 4, 1);
		graph16.add_edge(3, 5, 1);
		//	graph16.add_edge(4, 1, 1);
		graph16.print_edges();
		graph16.topological_sort().print_edges();


		std::cout << "\n\n\n===========Spanning tree====================\n";
		AdjMatrix graph17(7);
		graph17.add_undirected_edge(0, 1, 1);
		graph17.add_undirected_edge(1, 2, 1);
		//	graph17.add_undirected_edge(2, 3, 1);
		//	graph17.add_undirected_edge(3, 4, 1);
		graph17.add_undirected_edge(4, 5, 1);
		//	graph17.add_undirected_edge(5, 6, 1);
		//	graph17.add_undirected_edge(6, 2, 1);
		graph17.add_undirected_edge(0, 4, 1);
		graph17.add_undirected_edge(0, 5, 1);
		graph17.add_undirected_edge(2, 5, 1);
		//	graph17.add_undirected_edge(0, 3, 1);
		graph17.add_undirected_edge(2, 4, 1);
		graph17.add_undirected_edge(3, 6, 1);

		std::cout << "\nGraph 17 (matrix):\n";
		graph17.print_edges();
		std::cout << std::endl;
		graph17.print_matrix();

		std::cout << "\n Number of components = " << components_number(graph17) << std::endl;

		AdjStruct graph18(7);
		graph18.add_undirected_edge(0, 1, 1);
		graph18.add_undirected_edge(1, 2, 1);
		graph18.add_undirected_edge(2, 3, 1);
		graph18.add_undirected_edge(3, 4, 1);
		graph18.add_undirected_edge(4, 5, 1);
		graph18.add_undirected_edge(5, 6, 1);
		graph18.add_undirected_edge(6, 2, 1);
		graph18.add_undirected_edge(0, 4, 1);
		graph18.add_undirected_edge(0, 5, 1);
		graph18.add_undirected_edge(2, 5, 1);
		graph18.add_undirected_edge(2, 4, 1);
		std::cout << "\nGraph 18 (struct):\n";
		convert_in_matrix(graph18).print_matrix();

		std::cout << std::endl << std::endl;
		std::size_t spanning_weight = 0;
		std::cout << "\n\n\nSpanning forest for graph17 (matrix):\n";
		spanning_tree(graph17, spanning_weight, compare_number_matrix).print_matrix();
		std::cout << "\nspanning tree weight = " << spanning_weight << std::endl;

		std::cout << std::endl << std::endl;
		std::cout << "\n\n\nSpanning tree for graph18 (struct):\n";
		spanning_tree(graph18, spanning_weight, compare_number_struct).print_edges();
		std::cout << "\nspanning tree weight = " << spanning_weight << std::endl;

		std::cout << "\n\n\nSpanning tree/forest for graph19 (random matrix):\n";
		AdjMatrix graph19 = generate_random_matrix(10, 20, undirected);
		/*for (std::size_t i = 0; i < 10; i++) {
			if (i != 1) {
				graph19.remove_undirected_edge(1, i);
			}

		}*/
		graph19.print_matrix();
		std::cout << "\nMin Spanning tree:\n";
		min_spanning_tree(graph19, spanning_weight).print_matrix();
		std::cout << "\nspanning tree weight = " << spanning_weight << std::endl;


		std::cout << "\n\n\nSpanning tree/forest for graph20:\n";
		AdjStruct graph20(6);
		graph20.add_undirected_edge(0, 1, 1);
		graph20.add_undirected_edge(0, 2, 1);
		graph20.add_undirected_edge(0, 3, 1);
		graph20.add_undirected_edge(1, 2, 1);
		graph20.add_undirected_edge(1, 4, 1);
		graph20.add_undirected_edge(2, 5, 1);
		graph20.add_undirected_edge(3, 4, 1);
		graph20.add_undirected_edge(3, 5, 1);
		graph20.add_undirected_edge(4, 5, 1);
		convert_in_matrix(graph20).print_matrix();
		std::cout << "\nSpanning tree:\n";
		AdjStruct graph20_tree = spanning_tree(graph20, spanning_weight, compare_weight_struct);
		graph20_tree.print_edges();
		convert_in_matrix(graph20_tree).print_matrix();
		std::cout << "\nspanning tree weight = " << spanning_weight << std::endl;

		std::cout << "\n\n\n=====================Min spanning tree:====================\n";

		/*
		for (std::size_t I = 0; I < 15; I++) {
			std::cout << "\n\nMatrix # " << I+1 << std::endl;
			AdjMatrix graph_matrix = generate_random_matrix(I + 15, I + 20, undirected);
			graph_matrix.print_matrix();
			std::cout << "\nMin spanning tree:\n";
			min_spanning_tree(graph_matrix, spanning_weight).print_matrix();
			std::cout << "weight = " << spanning_weight << std::endl;

			std::cout << "\n\nStruct # " << I+1 << std::endl;
			AdjStruct graph_struct = generate_random_structure(I + 15, I + 20, undirected);
			convert_in_matrix(graph_struct).print_matrix();
			std::cout << "\nMin spanning tree:\n";
			AdjStruct struct_min = min_spanning_tree(graph_struct, spanning_weight);
			convert_in_matrix(struct_min).print_matrix();
			std::cout << "weight = " << spanning_weight << std::endl;
		}
		*/

		for (std::size_t I = 1; I < 6; I++) {
			std::cout << "\n\nMatrix # " << I << std::endl;
			AdjMatrix graph_matrix = generate_random_matrix(I + 1, I + floor(I / 2), undirected);
			graph_matrix.print_matrix();
			std::cout << "\nMin spanning tree:\n";
			min_spanning_tree(graph_matrix, spanning_weight).print_matrix();
			std::cout << "weight = " << spanning_weight << std::endl;

			std::cout << "\n\nStruct # " << I << std::endl;
			AdjStruct graph_struct = generate_random_structure(I + 1, I + floor(I / 2), undirected);
			convert_in_matrix(graph_struct).print_matrix();
			std::cout << "\nMin spanning tree:\n";
			AdjStruct struct_min = min_spanning_tree(graph_struct, spanning_weight);
			convert_in_matrix(struct_min).print_matrix();
			std::cout << "weight = " << spanning_weight << std::endl;
		}

		std::cout << "\n\n\nConnected components of graph18:\n";
		std::vector<std::vector<std::size_t>> connected_structure = connected_components(graph18);
		print_connected_components(connected_structure);
		std::cout << std::endl;
		graph17.print_edges();
	}
	std::cout << "\n\nTHE END!!!\n\n";
	std::system("pause");
	return 0;
}


void print_menu(bool is_structure = 0) {
	std::cout << "\n============MENU============\n" <<
		"0\tchoose realization\n" <<
		"1\tadd edge\n" <<
		"2\tremove edge\n" <<
		"3\treinitialize with random graph\n" <<
		"4\tprint \n";
		if (is_structure == false) {
			std::cout << "5\tconvert into Adjacent Structure\n";
		}
		else {
			std::cout << "5\tconvert into Adjacent Matrix\n";
		}

	std::cout << "6\tcheck connectivity\n" <<
		"7\tfind connected components\n" <<
		"8\tcheck acyclicity\n" <<
		"9\tcheck if graph is a tree\n" <<
		"10\tbuild transitive closure of graph\n" <<
		"11\tDFS\n" <<
		"12\tBFS\n" <<
		"13\tmin path\n" <<
		"14\ttopological sort\n" <<
		"15\tbuild spanning tree\n" <<
		"16\tbuild min spanning tree (for undirected graphs)\n" <<
		"===========================\n\n";
}

Edge get_edge(bool with_weight = true) {
	Edge new_edge;
	std::cout << "Enter start vertex (>= 0): ";
	std::cin >> new_edge.start_vertex;
	std::cout << "Enter end vertex (>= 0): ";
	std::cin >> new_edge.end_vertex;
	if (with_weight) {
		std::cout << "Enter edge weight (> 0): ";
		std::cin >> new_edge.weight;
	}


	return new_edge;
}


template<typename T>
void clean_matrix(T** matrix, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		delete[] matrix[i];
	}
	delete[]matrix;
}




void interactive() {
	while (true) {
		std::cout << "0 - exit, 1 - Adjacent Matrix, 2 - Adjacent Structure\n";
		short int realization = 1;
		std::cin >> realization;
		if (realization == 0) { return; }
		if (realization == 1) {
			std::cout << "Enter number of vertex (>0):";
			std::size_t size;
			std::cin >> size;
			AdjMatrix graph(size);
			std::cout << "graph created!\n";
			short int action = 1;
			while (action > 0) {
				print_menu(0);
				std::cin >> action;
				switch (action) {
				case 1: { // add adge
					short int direct;
					std::cout << "0 - directed, 1 - undirected edge: ";
					std::cin >> direct;
					Edge new_edge = get_edge();

					if (direct == 0) {
						graph.add_edge(new_edge.start_vertex, new_edge.end_vertex, new_edge.weight);
						std::cout << "directed edge added\n";
					}
					else {
						graph.add_undirected_edge(new_edge.start_vertex, new_edge.end_vertex, new_edge.weight);
						std::cout << "undirected edge added\n";
					}
					
					break;
				}
				case 2: { // remove edge
					short int direct;
					std::cout << "0 - directed, 1 - undirected edge: ";
					std::cin >> direct;
					Edge new_edge = get_edge(false);

					if (direct == 0) {
						graph.remove_edge(new_edge.start_vertex, new_edge.end_vertex);
						std::cout << "directed edge removed\n";
					}
					else {
						graph.remove_undirected_edge(new_edge.start_vertex, new_edge.end_vertex);
						std::cout << "undirected edge removed\n";
					}
					break;
				}
				case 3: {
					graph.clean();
					int size, edges, direct;
					std::cout << "Enter number of vertex: ";
					std::cin >> size;
					std::cout << "Enter number of edges: ";
					std::cin >> edges;
					std::cout << "0 - directed, 1 - undirected: ";
					std::cin >> direct;
					graph = generate_random_matrix(size, edges, direct);
					break;
				}
				case 4: { // print
					short int presentation;
					std::cout << "0 - matrix, 1 - edges: ";
					std::cin >> presentation;
					if (presentation == 0) {
						graph.print_matrix();
					}
					else {
						graph.print_edges();
					}
					break;
				}
				case 5: {
					std::cout << "Converted:\n";
					convert_in_struct(graph).print_edges();
					break;
				}
				case 6: {
					if (graph.is_connected() == true) {
						std::cout << "graph is connected\n";
					}
					else {
						std::cout << "graph isn't connected\n";
					}
					break;
				}
				case 7: {
					std::vector<std::vector<std::size_t>> connected_structure = connected_components(graph);
					print_connected_components(connected_structure);
					break;
				}
				case 8: {
					if (graph.is_cycles() == true) {
						std::cout << "graph has cycle\n";
					}
					else {
						std::cout << "graph hasn't cycle\n";
					}
					break;
				}
				case 9: {
					if (graph.is_tree() == true) {
						std::cout << "graph is a tree\n";
					}
					else {
						std::cout << "graph isn't a tree\n";
					}
					break;
				}
				case 10: {
					std::cout << "\nTransitive closure:\n";
					graph.transitive_closure();
					graph.print_edges();
					break;
				}
				case 11: { // DFS
					int vertex = -1, priority;
					std::cout << "-1 - DFS all, 0 - from vertex\n";
					std::cin >> vertex;
					if (vertex == 0) {
						std::cout << "Enter number of vertex: ";
						std::cin >> vertex;
					}
					std::cout << "priority: 0 - vertex number, 1 - weight\n";
					std::cin >> priority;
					bool (*order_func)(GraphNode&, GraphNode&) = priority == 0 ? compare_number_matrix : compare_weight_matrix;
					if (vertex == -1) {
						graph.depth_search_all(process_print, false, order_func);
					}
					else {
						graph.depth_search_one_component(vertex, process_print, false, order_func);
					}
					break;
				}
				case 12: { // BFS
					int vertex = -1, priority;
					std::cout << "-1 - BFS all, 0 - from one vertex\n";
					std::cin >> vertex;
					if (vertex == 0) {
						std::cout << "Enter number of vertex: ";
						std::cin >> vertex;
					}
					std::cout << "priority: 0 - vertex number, 1 - weight\n";
					std::cin >> priority;
					bool (*order_func)(GraphNode&, GraphNode&) = priority == 0 ? compare_number_matrix : compare_weight_matrix;
					if (vertex == -1) {
						graph.breadth_search_all(process_print, order_func);
					}
					else {
						graph.breadth_search_one_component(vertex, process_print, order_func);
					}
					break;
				}
				case 13: { // min paths
					std::cout << "1 - between two vertices, 2 - from one to all, 3 - from all to all: ";
					int next;
					std::cin >> next;
					if (next == 1) {
						Edge vertices = get_edge(false);
						Path path = get_path_between_two(graph, vertices.start_vertex, vertices.end_vertex);
						print_path(path, vertices.start_vertex, vertices.end_vertex);
					}
					else if (next == 2) {
						std::cout << "Enter vertix: ";
						std::size_t vertex;
						std::cin >> vertex;
						std::vector<Path> path_structure = get_path_from_one(graph, vertex);
						print_path_structure(path_structure, vertex);
					}
					else if (next == 3) {
						Path** path_matrix = get_path_from_all(graph);
						print_path_all(path_matrix, graph.size);
						clean_matrix(path_matrix, graph.size);
					}
					break;
				}
				case 14: { // topological sort
					AdjMatrix sorted_graph = graph.topological_sort();
					std::cout << "Sorted graph:\n";
					sorted_graph.print_edges();
					break;
				}
				case 15: { // spanning tree
					int priority;
					std::cout << "priority: 0 - vertex number, 1 - weight\n";
					std::cin >> priority;
					bool (*order_func)(GraphNode&, GraphNode&) = priority == 0 ? compare_number_matrix : compare_weight_matrix;
					std::size_t weight = 0;

					std::cout << "Spanning tree:\n";
					AdjMatrix spanning_graph = spanning_tree(graph, weight, order_func);
					spanning_graph.print_edges();
					std::cout << std::endl;
					spanning_graph.print_matrix();
					std::cout << "weight = " << weight << std::endl;
					break;
				}
				case 16: { // min spanning tree
					std::size_t weight = 0;

					std::cout << "Minimal spanning tree:\n";
					AdjMatrix min_spanning_graph = min_spanning_tree(graph, weight);
					min_spanning_graph.print_edges();
					std::cout << std::endl;
					min_spanning_graph.print_matrix();
					std::cout << "weight = " << weight << std::endl;
					break;
				}
				default:break;

				}
			}
			graph.clean();
		}
		else if (realization == 2) {
		std::cout << "Enter number of vertex (>0):";
		std::size_t size;
		std::cin >> size;
		AdjStruct graph(size);
		std::cout << "graph created!\n";
		short int action = 1;
		while (action > 0) {
			print_menu(1);
			std::cin >> action;
			switch (action) {
			case 1: { // add adge
				short int direct;
				std::cout << "0 - directed, 1 - undirected edge: ";
				std::cin >> direct;
				Edge new_edge = get_edge();

				if (direct == 0) {
					graph.add_edge(new_edge.start_vertex, new_edge.end_vertex, new_edge.weight);
					std::cout << "directed edge added\n";
				}
				else {
					graph.add_undirected_edge(new_edge.start_vertex, new_edge.end_vertex, new_edge.weight);
					std::cout << "undirected edge added\n";
				}

				break;
			}
			case 2: { // remove edge
				short int direct;
				std::cout << "0 - directed, 1 - undirected edge: ";
				std::cin >> direct;
				Edge new_edge = get_edge(false);

				if (direct == 0) {
					graph.remove_edge(new_edge.start_vertex, new_edge.end_vertex);
					std::cout << "directed edge removed\n";
				}
				else {
					graph.remove_undirected_edge(new_edge.start_vertex, new_edge.end_vertex);
					std::cout << "undirected edge removed\n";
				}
				break;
			}
			case 3: {
				graph.clean();
				int size, edges, direct;
				std::cout << "Enter number of vertex: ";
				std::cin >> size;
				std::cout << "Enter number of edges:" ;
				std::cin >> edges;
				std::cout << "0 - directed, 1 - undirected: ";
				std::cin >> direct;
				graph = generate_random_structure(size, edges, direct);
				break;
			}
			case 4: { // print
				short int presentation;
				std::cout << "0 - matrix, 1 - edges: ";
				std::cin >> presentation;
				if (presentation == 0) {
					graph.print_matrix();
				}
				else {
					graph.print_edges();
				}
				break;
			}
			case 5: {
				std::cout << "Converted:\n";
				convert_in_matrix(graph).print_matrix();
				break;
			}
			case 6: {
				if (graph.is_connected() == true) {
					std::cout << "graph is connected\n";
				}
				else {
					std::cout << "graph isn't connected\n";
				}
				break;
			}
			case 7: {
				std::vector<std::vector<std::size_t>> connected_structure = connected_components(graph);
				print_connected_components(connected_structure);
				break;
			}
			case 8: {
				if (graph.is_cycles() == true) {
					std::cout << "graph has cycle\n";
				}
				else {
					std::cout << "graph hasn't cycle\n";
				}
				break;
			}
			case 9: {
				if (graph.is_tree() == true) {
					std::cout << "graph is a tree\n";
				}
				else {
					std::cout << "graph isn't a tree\n";
				}
				break;
			}
			case 10: {
				std::cout << "\nTransitive closure:\n";
				graph.transitive_closure();
				graph.print_edges();
				break;
			}
			case 11: { // DFS
				int vertex = -1, priority;
				std::cout << "-1 - DFS all, 0 - from vertex\n";
				std::cin >> vertex;
				if (vertex == 0) {
					std::cout << "Enter number of vertex: ";
					std::cin >> vertex;
				}
				std::cout << "priority: 0 - vertex number, 1 - weight\n";
				std::cin >> priority;
				bool (*order_func)(GraphNode*, GraphNode*) = priority == 0 ? compare_number_struct : compare_weight_struct;
				if (vertex == -1) {
					graph.depth_search_all(process_print, false, order_func);
				}
				else {
					graph.depth_search_one_component(vertex, process_print, false, order_func);
				}
				break;
			}
			case 12: { // BFS
				int vertex = -1, priority;
				std::cout << "-1 - BFS all, 0 - from one vertex\n";
				std::cin >> vertex;
				if (vertex == 0) {
					std::cout << "Enter number of vertex: ";
					std::cin >> vertex;
				}
				std::cout << "priority: 0 - vertex number, 1 - weight\n";
				std::cin >> priority;
				bool (*order_func)(GraphNode*, GraphNode*) = priority == 0 ? compare_number_struct : compare_weight_struct;
				if (vertex == -1) {
					graph.breadth_search_all(process_print, order_func);
				}
				else {
					graph.breadth_search_one_component(vertex, process_print, order_func);
				}
				break;
			}
			case 13: { // min paths
				std::cout << "1 - between two vertices, 2 - from one to all, 3 - from all to all: ";
				int next;
				std::cin >> next;
				if (next == 1) {
					Edge vertices = get_edge(false);
					Path path = get_path_between_two(graph, vertices.start_vertex, vertices.end_vertex);
					print_path(path, vertices.start_vertex, vertices.end_vertex);
				}
				else if (next == 2) {
					std::cout << "Enter vertix: ";
					std::size_t vertex;
					std::cin >> vertex;
					std::vector<Path> path_structure = get_path_from_one(graph, vertex);
					print_path_structure(path_structure, vertex);
				}
				else if (next == 3) {
					Path** path_matrix = get_path_from_all(graph);
					print_path_all(path_matrix, graph.size);
					clean_matrix(path_matrix, graph.size);
				}
				break;
			}
			case 14: { // topological sort
				AdjStruct sorted_graph = graph.topological_sort();
				std::cout << "Sorted graph:\n";
				sorted_graph.print_edges();
				break;
			}
			case 15: { // spanning tree
				int priority;
				std::cout << "priority: 0 - vertex number, 1 - weight\n";
				std::cin >> priority;
				bool (*order_func)(GraphNode*, GraphNode*) = priority == 0 ? compare_number_struct : compare_weight_struct;
				std::size_t weight = 0;

				std::cout << "Spanning tree:\n";
				AdjStruct spanning_graph = spanning_tree(graph, weight, order_func);
				spanning_graph.print_edges();
				std::cout << std::endl;
				spanning_graph.print_matrix();
				std::cout << "weight = " << weight << std::endl;
				break;
			}
			case 16: { // min spanning tree
				std::size_t weight = 0;

				std::cout << "Minimal spanning tree:\n";
				AdjStruct min_spanning_graph = min_spanning_tree(graph, weight);
				min_spanning_graph.print_edges();
				std::cout << std::endl;
				min_spanning_graph.print_matrix();
				std::cout << "weight = " << weight << std::endl;
				break;
			}
			default:break;

			}
		}
		graph.clean();
		}
	}
}


void demonstrative() {
	std::size_t const SIZE = 6;
	std::cout << "\n1. Create graph with " << SIZE <<" vertices and add some undirected edges:\n";
	AdjMatrix graph_matrix(SIZE);
	AdjStruct graph_struct(SIZE);
	for (std::size_t i = 0; i < SIZE - 1; i++) {
		graph_matrix.add_undirected_edge(i, i + 1, rand()%9 + 1);
		graph_struct.add_undirected_edge(i, i + 1, rand() % 9 + 1);
	}
	std::cout << "Graph matrix:\n";
	graph_matrix.print_matrix();
	std::cout << "\nGraph structure:\n";
	graph_struct.print_edges();


	std::cout << "\n2. Remove undirected edge (0, 1):\n";
	graph_matrix.remove_undirected_edge(0, 1);
	graph_struct.remove_undirected_edge(0, 1);

	std::cout << "Graph matrix:\n";
	graph_matrix.print_matrix();
	std::cout << std::endl;
	graph_matrix.print_edges();
	std::cout << "\nGraph structure:\n";
	graph_struct.print_edges();
	std::cout << std::endl;
	graph_struct.print_matrix();


	std::cout << "\n3. Converting:\n";
	std::cout << "Convert graph matrix into struct:\n";
	convert_in_struct(graph_matrix).print_edges();

	std::cout << "\nConvert graph struct into matrix:\n";
	convert_in_matrix(graph_struct).print_matrix();

	std::cout << "\n4. Check on connectivity:\n";
	if (graph_matrix.is_connected() == true) {
		std::cout << "graph_matrix is connected\n";
	}
	else {
		std::cout << "graph_matrix isn't connected\n";
	}

	if (graph_struct.is_connected() == true) {
		std::cout << "graph_struct is connected\n";
	}
	else {
		std::cout << "graph_struct isn't connected\n";
	}


	std::cout << "\n5. Connected components:\n";
	std::cout << "Graph matrix:\n";
	std::vector<std::vector<std::size_t>> connected_structure = connected_components(graph_matrix);
	print_connected_components(connected_structure);

	std::cout << "\nGraph struct:\n";
	connected_structure = connected_components(graph_matrix);
	print_connected_components(connected_structure);


	std::cout << "\n6. Checking for cycle:\n";
	if (graph_matrix.is_cycles() == true) {
		std::cout << "graph_matrix has cycle\n";
	}
	else {
		std::cout << "graph_matrix hasn't cycle\n";
	}

	if (graph_struct.is_cycles() == true) {
		std::cout << "graph_struct has cycle\n";
	}
	else {
		std::cout << "graph_struct hasn't cycle\n";
	}


	std::cout << "\n7. Checking if tree:\n";
	if (graph_matrix.is_cycles() == true) {
		std::cout << "graph_matrix is a tree\n";
	}
	else {
		std::cout << "graph_matrix isn't a tree\n";
	}

	if (graph_struct.is_cycles() == true) {
		std::cout << "graph_struct is a tree\n";
	}
	else {
		std::cout << "graph_struct isn't a tree\n";
	}

	std::cout << "\n8. DFS:\n";
	std::cout << "\ngraph matrix starting from 1 according to vertex order: ";
	graph_matrix.depth_search_one_component(1, process_print, false, compare_number_matrix);
	std::cout << "\ngraph matrix starting from 1 according to edge weight order: ";
	graph_matrix.depth_search_one_component(1, process_print, false, compare_weight_matrix);
	std::cout << "\ngraph matrix all according to vertex order: ";
	graph_matrix.depth_search_all(process_print, false, compare_number_matrix);
	std::cout << "\ngraph matrix all according to edge weight order: ";
	graph_matrix.depth_search_all(process_print, false, compare_weight_matrix);

	std::cout << "\n\ngraph struct starting from 1 according to vertex order: ";
	graph_struct.depth_search_one_component(1, process_print, false, compare_number_struct);
	std::cout << "\ngraph struct starting from 1 according to edge weight order: ";
	graph_struct.depth_search_one_component(1, process_print, false, compare_weight_struct);
	std::cout << "\ngraph struct all according to vertex order: ";
	graph_struct.depth_search_all(process_print, false, compare_number_struct);
	std::cout << "\ngraph struct all according to edge weight order: ";
	graph_struct.depth_search_all(process_print, false, compare_weight_struct);
	

	std::cout << "\n\n9. BFS:\n";
	std::cout << "\ngraph matrix starting from 1 according to vertex order: ";
	graph_matrix.breadth_search_one_component(1, process_print, compare_number_matrix);
	std::cout << "\ngraph matrix starting from 1 according to edge weight order: ";
	graph_matrix.breadth_search_one_component(1, process_print, compare_weight_matrix);
	std::cout << "\ngraph matrix all according to vertex order: ";
	graph_matrix.breadth_search_all(process_print, compare_number_matrix);
	std::cout << "\ngraph matrix all according to edge weight order: ";
	graph_matrix.breadth_search_all(process_print, compare_weight_matrix);

	std::cout << "\n\ngraph struct starting from 1 according to vertex order: ";
	graph_struct.breadth_search_one_component(1, process_print, compare_number_struct);
	std::cout << "\ngraph struct starting from 1 according to edge weight order: ";
	graph_struct.breadth_search_one_component(1, process_print, compare_weight_struct);
	std::cout << "\ngraph struct all according to vertex order: ";
	graph_struct.breadth_search_all(process_print, compare_number_struct);
	std::cout << "\ngraph struct all according to edge weight order: ";
	graph_struct.breadth_search_all(process_print, compare_weight_struct);
	
	std::cout << "\n\n10. Min paths:\n";
	std::cout << "Graph matrix:\n";
	std::cout << "From 1 to 4:\n";
	Path path = get_path_between_two(graph_matrix, 1, 4);
	print_path(path, 1, 4);

	std::cout << "\nFrom 2 to all:\n";
	std::vector<Path> path_structure = get_path_from_one(graph_matrix, 2);
	print_path_structure(path_structure, 2);

	std::cout << "\nFrom all to all:\n";
	Path** path_matrix = get_path_from_all(graph_matrix);
	print_path_all(path_matrix, graph_matrix.size);
	clean_matrix(path_matrix, graph_matrix.size);

	std::cout << "Graph struct:\n";
	std::cout << "From 1 to 4:\n";
	path = get_path_between_two(graph_struct, 1, 4);
	print_path(path, 1, 4);

	std::cout << "\nFrom 2 to all:\n";
	path_structure = get_path_from_one(graph_struct, 2);
	print_path_structure(path_structure, 2);

	std::cout << "\nFrom all to all:\n";
	path_matrix = get_path_from_all(graph_struct);
	print_path_all(path_matrix, graph_struct.size);
	clean_matrix(path_matrix, graph_struct.size);


	std::cout << "\n\n11. Spanning tree:\n";
	std::cout << "For graph_matrix according to vertices number order:\n";
	std::size_t weight = 0;
	AdjMatrix spanning_graph_matrix = spanning_tree(graph_matrix, weight, compare_number_matrix);
	spanning_graph_matrix.print_edges();
	std::cout << std::endl;
	spanning_graph_matrix.print_matrix();
	std::cout << "weight = " << weight << std::endl;
	spanning_graph_matrix.clean();

	std::cout << "\nFor graph_matrix according to edges weight order:\n";
	spanning_graph_matrix = spanning_tree(graph_matrix, weight, compare_weight_matrix);
	spanning_graph_matrix.print_edges();
	std::cout << std::endl;
	spanning_graph_matrix.print_matrix();
	std::cout << "weight = " << weight << std::endl;
	spanning_graph_matrix.clean();


	std::cout << "\nFor graph_struct according to vertices number order:\n";
	AdjStruct spanning_graph_struct = spanning_tree(graph_struct, weight, compare_number_struct);
	spanning_graph_struct.print_edges();
	std::cout << std::endl;
	spanning_graph_struct.print_matrix();
	std::cout << "weight = " << weight << std::endl;
	spanning_graph_struct.clean();

	std::cout << "\nFor graph_struct according to edges weight order:\n";
	spanning_graph_struct = spanning_tree(graph_struct, weight, compare_weight_struct);
	spanning_graph_struct.print_edges();
	std::cout << std::endl;
	spanning_graph_struct.print_matrix();
	std::cout << "weight = " << weight << std::endl;
	spanning_graph_struct.clean();


	std::cout << "\n\n12. Min spanning tree:\n";

	std::cout << "\nFor graph_matrix:\n";
	spanning_graph_matrix = min_spanning_tree(graph_matrix, weight);
	spanning_graph_matrix.print_edges();
	std::cout << std::endl;
	spanning_graph_matrix.print_matrix();
	std::cout << "weight = " << weight << std::endl;

	std::cout << "\nFor graph_struct:\n";
	spanning_graph_struct = min_spanning_tree(graph_struct, weight);
	spanning_graph_struct.print_edges();
	std::cout << std::endl;
	spanning_graph_struct.print_matrix();
	std::cout << "weight = " << weight << std::endl;

	std::cout << "\n\n13. Topological sort:\n";
	std::cout << "Remove some directed edges to make graph_matrix directed:\n";
	for (std::size_t i = 1; i < SIZE - 1; i++) {
		graph_matrix.remove_edge(i + 1, i);
		graph_struct.remove_edge(i + 1, i);
	}
	graph_matrix.print_edges();

	std::cout << "\nsorted graph_matrix:\n";
	AdjMatrix sorted_graph_matrix = graph_matrix.topological_sort();
	sorted_graph_matrix.print_edges();
	sorted_graph_matrix.clean();

	std::cout << "Remove some directed edges to make graph_struct directed:\n";
	graph_struct.print_edges();

	std::cout << "\nsorted graph_struct:\n";
	AdjStruct sorted_graph_struct = graph_struct.topological_sort();
	sorted_graph_struct.print_edges();
	sorted_graph_struct.clean();


	std::cout << "\n\n14. Transitive closure:\n";
	std::cout << "graph_matrix:\n";
	graph_matrix.transitive_closure();
	graph_matrix.print_edges();
	graph_matrix.clean();

	std::cout << "\ngraph_struct:\n";
	graph_struct.transitive_closure();
	graph_struct.print_edges();
	graph_struct.clean();

	std::cout << "\n\n15. Generate directed graph with 5 vertices and 7 edges:\n";
	std::cout << "graph_matrix:\n";
	generate_random_matrix(5, 7).print_matrix();

	std::cout << "graph_struct:\n";
	generate_random_structure(5, 7).print_edges();

}
//}
	

