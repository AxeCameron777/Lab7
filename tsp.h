#include "graph.h"

#include <vector>
#include <algorithm>
#include <iostream>

std::vector<int> tsp_ls(const Graph& graph, int start_vertex);

std::vector<int> tsp_greed(const Graph& graph, int start_vertex);

std::vector<int> tsp_native(const Graph& graph, int start_vertex);

std::vector<int> tsp_bnb(const Graph& graph, int start_vertex);