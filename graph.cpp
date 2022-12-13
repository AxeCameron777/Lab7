#include "graph.h"

#include <stdexcept>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

Graph::Graph(initializer_list<tuple<int, int, double>> edges) {
    for (const auto& e : edges) {
        add_edge(get<0>(e), get<1>(e), get<2>(e));
    }
}

void Graph::add_vertex(int vertex) {
    if (!has_vertex(vertex)) {
        vertices[vertex] = std::map<int, double>();
    }
}

void Graph::add_edge(int start_vertex, int end_vertex, double weight) {
    add_vertex(start_vertex);
    add_vertex(end_vertex);
    vertices[start_vertex][end_vertex] = weight;
    vertices[end_vertex][start_vertex] = weight;
}

double Graph::min_two_weight_sum(int vertex) const{
    auto a = this->get_adjacent_edges(vertex);
    std::vector<double> weights = {};
    for (auto pair : a)
        weights.push_back(pair.second);
    double b[2] = { 0, 0 };
    std::partial_sort_copy(weights.begin(), weights.end(), b, b + 2);
    return b[0] + b[1];
}

std::vector<int> Graph::get_vertices() const {
    std::vector<int> result;
    for (const auto& p : vertices) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<int> Graph::get_adjacent_vertices(int src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return std::vector<int> {};
    }
    vector<int> result;
    for (const auto& p : it->second) {
        result.push_back(p.first);
    }
    return result;
}

vector<pair<int, double>> Graph::get_adjacent_edges(int src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return vector<pair<int, double>> {};
    }
    vector<pair<int, double>> result;
    for (const auto& p : it->second) {
        result.push_back(make_pair(p.first, p.second));
    }
    return result;
}

bool Graph::has_vertex(int vertex) const {
    return (vertices.find(vertex) != vertices.end());
}

bool Graph::has_edge(int start_vertex, int end_vertex) const {
    const auto it = vertices.find(start_vertex);
    if (it == vertices.end()) {
        return false;
    }
    return (it->second.find(end_vertex) != it->second.end());
}

double Graph::edge_weight(int start_vertex, int end_vertex) const {
    const auto it_s = vertices.find(start_vertex);
    if (it_s == vertices.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    const auto it_e = it_s->second.find(end_vertex);
    if (it_e == it_s->second.end()) {
        std::cout << start_vertex << ' ' << end_vertex;
        throw invalid_argument("Edge doesn't exist");
    }
    return it_e->second;
}

void Graph::remove_vertex(int vertex) {
    /// Remove adjacent edges.
    auto adjacent_vertices = get_adjacent_vertices(vertex);
    for (const auto& adj_v : adjacent_vertices) {
        remove_edge(adj_v, vertex);
    }
    /// Remove the vertex itself.
    vertices.erase(vertex);
}

void Graph::remove_edge(int start_vertex, int end_vertex) {
    auto it_s = vertices.find(start_vertex);
    if (it_s != vertices.end()) {
        it_s->second.erase(end_vertex);
    }
    auto it_e = vertices.find(end_vertex);
    if (it_e != vertices.end()) {
        it_e->second.erase(start_vertex);
    }
}



bool is_valid_path(const Graph& g, const std::vector<int>& path) {
    if (size(path) == 0)
        return false;
    for (int i = 0; i < path.size() - 1; i++) {
        std::vector<int> neigh = g.get_adjacent_vertices(path[i]);
        if (find(neigh.begin(), neigh.end(), path[i + 1]) == neigh.end())
            return false;
    }
    std::vector<int> neigh = g.get_adjacent_vertices(path[path.size() - 1]);
    return find(neigh.begin(), neigh.end(), path[0]) != neigh.end();
}

double path_length(const Graph& g, const std::vector<int>& path) {
    double distance = 0;
    if (size(path) == 0 || !is_valid_path(g, path))
        return 20000000;
    for (int i = 0; i < size(path) - 1; i++)
        distance += g.edge_weight(path[i], path[i + 1]);
    distance += g.edge_weight(path[path.size() - 1], path[0]);
    return distance;
}

std::vector<int> MinPath(const Graph& g, const std::vector<int>& path1, const std::vector<int>& path2) {
    if (path1.size() == 0)
        return path2;
    double d1 = path_length(g, path2), d2 = path_length(g, path1);
    if (d1 < d2) return path2;
    else return path1;
}

Graph random_graph(int vertexes, int min_distance, int max_distance) {
    Graph g;
    std::uniform_real_distribution<> simple_distance(min_distance, max_distance);
    for (int i = 1; i <= vertexes - 1; i++)
        for (int j = i + 1; j <= vertexes; j++)
            g.add_edge(i, j, simple_distance(gen));
    return g;
}
