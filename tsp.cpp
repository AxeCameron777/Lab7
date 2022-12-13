#include "tsp.h"

using namespace std;

double MAX_DISTANCE = 2000000;
vector<vector<int> > permutations;

vector<int> tsp_greed(const Graph& graph, int start_vertex) {
	int n = size(graph.get_vertices());
	int current = start_vertex;
	vector<int> path = { start_vertex };
	while (size(path) != n) {
		vector<int> s = {};
		for (auto vertex : graph.get_adjacent_vertices(current))
			if (find(path.begin(), path.end(), vertex) == path.end())
				s.push_back(vertex);
		if (size(s) == 0) return {};
		int min_vertex = s[0];
		int min_dist = graph.edge_weight(current, s[0]);
		for (int i = 1; i < size(s); i++)
			if (min_dist > graph.edge_weight(current, s[i])) {
				min_vertex = s[i];
				min_dist = graph.edge_weight(current, s[i]);
			}
		path.push_back(min_vertex);
		current = min_vertex;
	}
	if (is_valid_path(graph, path)) return path;
	else return {};
}

vector<int> transform(vector<int> path, int a, int b, int c, int d) {
	vector<int> output = { };
	for (int i = 0; path[i] != a; i++)
		output.push_back(path[i]);
	output.push_back(a);
	bool flag = false;
	for (int i = size(path) - 1; i >= 0; i--) {
		if (path[i] == c) flag = true;
		if (flag) output.push_back(path[i]);
		if (path[i] == b) break;
	}
	for (int i = 0; i < size(path); i++) {
		if (path[i] == d) flag = false;
		if (!flag) output.push_back(path[i]);
	}
	return output;
}

vector<int> TwoOptImprove(const Graph& graph, vector<int> path) {
	for (int i = 0; i < size(path) - 1; i++)
		for (int j = 0; j < size(path) - 1; j++)
			if (i != j && i != j + 1 && i + 1 != j && i + 1 != j + 1){
				double old_weight = graph.edge_weight(path[i], path[i + 1]) + graph.edge_weight(path[j], path[j + 1]);
				double new_weight = old_weight + 1;
				if (graph.has_edge(path[i], path[i + 1]) && graph.has_edge(path[j], path[j + 1]))
					new_weight = graph.edge_weight(path[i], path[j]) + graph.edge_weight(path[i + 1], path[j + 1]);
				if (new_weight < old_weight)
					return transform(path, path[i], path[i + 1], path[j], path[j + 1]);
			}
	return path;
}

vector<int> tsp_ls(const Graph& graph, int start_vertex) {
	if (size(graph.get_vertices()) <= 1)
		return {};
	vector<int> path = tsp_greed(graph, start_vertex);
	if (size(path) == 0) return {};
	while (true) {
		vector<int> improved_path = TwoOptImprove(graph, path);
		if (path_length(graph, improved_path) < path_length(graph, path)) path = improved_path;
		else if (is_valid_path(graph, path)) return path;
		else return {};
	}
}

void find_all_permutations(const vector<int>& alphabet, vector<int> existed) {
	if (existed.size() == alphabet.size())
		permutations.push_back(existed);
	else
		for (auto elem : alphabet)
			if (find(existed.begin(), existed.end(), elem) == existed.end()) {
				vector<int> copy = existed;
				copy.push_back(elem);
				find_all_permutations(alphabet, copy);
			}
}

vector<int> tsp_native(const Graph& graph, int start_vertex) {
	permutations = {};
	find_all_permutations(graph.get_vertices(), { start_vertex });
	vector<int> min_path = {};
	double min_distance = MAX_DISTANCE;
	for (auto path : permutations)
		if (is_valid_path(graph, path)) {
			double len = path_length(graph, path);
			if (len < min_distance) {
				min_distance = len;
				min_path = path;
			}
		}
	if (min_path.size() == graph.get_vertices().size())
		return min_path;
	return {};
}

double LowerBound(const Graph& graph, vector<int> visited) {
	double summ = 0;
	for (int i = 1; i < size(visited); i++)
		summ += graph.edge_weight(visited[i - 1], visited[i]);
	for (auto vertex : graph.get_vertices())
		if (find(visited.begin(), visited.end(), vertex) == visited.end())
			summ += graph.min_two_weight_sum(vertex) / 2.0;
	return summ;
}

vector<int> bnb(const Graph& graph, vector<int> visited, vector<int> best_path) {
	if (visited.size() == graph.get_vertices().size()) {
		return MinPath(graph, best_path, visited);
	}
	for (auto vertex : graph.get_vertices())
		if (find(visited.begin(), visited.end(), vertex) == visited.end() && graph.has_edge(visited[size(visited) - 1], vertex)) {
			vector<int> vnext;
			vnext.insert(vnext.end(), visited.begin(), visited.end());
			vnext.push_back(vertex);
			if (LowerBound(graph, vnext) < path_length(graph, best_path)) {
				auto path = bnb(graph, vnext, best_path);
				best_path = MinPath(graph, best_path, path);
			}
		}
	return best_path;
}

vector<int> tsp_bnb(const Graph& graph, int start_vertex) {
	auto path = bnb(graph, { start_vertex }, {});
	if (is_valid_path(graph, path))
		return path;
	else return {};
}