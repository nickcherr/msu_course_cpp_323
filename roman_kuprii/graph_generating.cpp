#include <random>
#include <vector>

#include "graph.hpp"
#include "graph_generating.hpp"

namespace {

double get_random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  return dis(gen);
}

constexpr double GREEN_TRASHOULD = 0.1;
constexpr double BLUE_TRASHOULD = 0.25;
constexpr double RED_TRASHOULD = 0.33;

using std::vector;

}  // namespace

namespace uni_cpp_practice {

namespace graph_generating {

void new_vertices_generation(Graph& work_graph,
                             const int& depth,
                             const int& new_vertices_num) {
  for (int current_depth = 0; current_depth <= depth; current_depth++) {
    const double probability =
        static_cast<double>(current_depth) / static_cast<double>(depth);
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        for (int iter = 0; iter < new_vertices_num; iter++) {
          if (get_random_number() > probability) {
            work_graph.add_vertex();
            work_graph.connect_vertices(
                vertex.get_id(),
                work_graph.get_vertices()[work_graph.get_vertices_num() - 1]
                    .get_id(),
                true);
          }
        }
  }
}

void add_blue_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  for (int current_depth = 1; current_depth <= graph_depth; current_depth++) {
    vector<Vertex> uni_depth_vertices;
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        uni_depth_vertices.emplace_back(vertex);

    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex : uni_depth_vertices) {
      if (adjacent_vertices[0] == INVALID_ID) {
        adjacent_vertices[0] = vertex.get_id();
      } else if (adjacent_vertices[1] == INVALID_ID) {
        adjacent_vertices[1] = vertex.get_id();
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (get_random_number() < BLUE_TRASHOULD)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], false);
      } else {
        adjacent_vertices[0] = adjacent_vertices[1];
        adjacent_vertices[1] = vertex.get_id();
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (get_random_number() < BLUE_TRASHOULD)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], false);
      }
    }
  }
}

void add_green_edges(Graph& work_graph) {
  for (const auto& start_vertex : work_graph.get_vertices())
    if (!work_graph.is_connected(start_vertex.get_id(), start_vertex.get_id()))
      if (get_random_number() < GREEN_TRASHOULD)
        work_graph.connect_vertices(start_vertex.get_id(),
                                    start_vertex.get_id(), false);
}

void add_red_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& start_vertex : work_graph.get_vertices()) {
    if (get_random_number() < RED_TRASHOULD) {
      if (start_vertex.depth + 2 <= graph_depth) {
        vector<VertexId> red_vertices_ids;
        for (const auto& end_vertex : work_graph.get_vertices()) {
          if (end_vertex.depth == start_vertex.depth + 2)
            if (!work_graph.is_connected(start_vertex.get_id(),
                                         end_vertex.get_id()))
              red_vertices_ids.emplace_back(end_vertex.get_id());
        }
        if (red_vertices_ids.size() > 0) {
          std::uniform_int_distribution<> distr(0, red_vertices_ids.size() - 1);
          work_graph.connect_vertices(start_vertex.get_id(),
                                      red_vertices_ids[distr(gen)], false);
        }
      }
    }
  }
}

void add_yellow_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& start_vertex : work_graph.get_vertices()) {
    const double probability = static_cast<double>(start_vertex.depth) /
                               static_cast<double>(graph_depth);
    if (get_random_number() < probability) {
      vector<VertexId> yellow_vertices_ids;
      for (const auto& end_vertex : work_graph.get_vertices()) {
        if (end_vertex.depth == start_vertex.depth + 1) {
          if (!work_graph.is_connected(start_vertex.get_id(),
                                       end_vertex.get_id()))
            yellow_vertices_ids.push_back(end_vertex.get_id());
        }
      }
      if (yellow_vertices_ids.size() > 0) {
        std::uniform_int_distribution<> distr(0,
                                              yellow_vertices_ids.size() - 1);
        work_graph.connect_vertices(start_vertex.get_id(),
                                    yellow_vertices_ids[distr(gen)], false);
      }
    }
  }
}

void paint_edges(Graph& work_graph) {
  add_blue_edges(work_graph);
  add_green_edges(work_graph);
  add_red_edges(work_graph);
  add_yellow_edges(work_graph);
}

}  // namespace graph_generating

}  // namespace uni_cpp_practice
