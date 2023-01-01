#ifndef GAME_GRAPH_H
#define GAME_GRAPH_H

#include <exception>
#include <map>
#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

namespace data {

/**
 * Gets thrown when a non-existent node is attempted to be accessed in a graph.
 *
 * @tparam Node The node type of the graph.
 */
template <class Node>
class MissingNode : public std::exception {
public:
    explicit MissingNode(Node n);
    const char* what() const noexcept override;

    /**
     * @return The value of the missing node.
     */
    Node getNode() const;

private:
    Node n;
};

/**
 * Graph is an implementation of a directed graph with a generic Node and Edge type. The graph's edges are represented
 * with a dense matrix.
 *
 * @tparam Node The type to use for the nodes. Must be able to be storable in an std::unordered_map. Different nodes
 * cannot have duplicate values.
 * @tparam Edge The type stored in each edge. Different edges can have the same value.
 */
template <class Node, class Edge>
class Graph {
public:
    Graph() = default;
    explicit Graph(std::vector<Node> nodes);

    /**
     * Adds a new node to the graph. It start without any edges to other nodes. Does nothing if the node is already
     * inserted.
     *
     * @param n The value to put in the node.
     */
    void insertNode(const Node& n);
    /**
     * Removes a node from the graph.
     *
     * @param node The node to remove.
     *
     * @throws MissingNode If node does not exist.
     */
    void removeNode(const Node& node);

    /**
     * Changes the value of an edge in the graph. This will either create a new edge between n1 and n2 if the edge does
     * not exist yet, or override the value of the edge if it does exist.
     *
     * @param n1 The value of the 'from' node.
     * @param n2 The value of the 'to' node.
     * @param e The value to write in the edge.
     *
     * @throws MissingNode If either n1 or n2 do not exist.
     */
    void setEdge(const Node& n1, const Node& n2, const Edge& e);
    /**
     * Removes an edge and it's value if the edge exists. If the edge does not exist, nothing will happen.
     *
     * @param n1 The value of the 'from' node.
     * @param n2 The value of the 'to' node.
     *
     * @throws MissingNode If either n1 or n2 do not exist.
     */
    void clearEdge(const Node& n1, const Node& n2);

    /**
     * Returns the value at a given edge.
     *
     * @param n1 The value of the 'from' node.
     * @param n2 The value of the 'to' node.
     * @return The optional value of the edge. If the edge does not exist, has_value() will be false.
     *
     * @throws MissingNode If either n1 or n2 do not exist.
     */
    std::optional<Edge> getEdge(const Node& n1, const Node& n2) const;
    /**
     * Checks if the given edge from n1 to n2 exists in the graph.
     *
     * @param n1 The value of the 'from' node.
     * @param n2 The value of the 'to' node.
     * @return True if the edge exists, false otherwise.
     *
     * @throws MissingNode If either n1 or n2 do not exist.
     */
    bool hasEdge(const Node& n1, const Node& n2) const;

    /**
     * Returns all edges that originate from node n1, along with their values and destinations.
     *
     * @param n1 The node from which the edges start.
     * @return A vector with edges. Also contains all nodes that it does not have an edge to. In this case, has_value()
     * will be false for that edge's (second) value.
     *
     * @throws MissingNode If n1 does not exist.
     */
    std::vector<std::pair<const Node&, const std::optional<Edge>&>> getEdges(const Node& n1) const;
    /**
     * Removes all edges. Does not de-allocate any memory.
     */
    void clearEdges();

private:
    std::size_t edgeId(const Node& n1, const Node& n2) const;

    std::queue<unsigned int> freeIds;
    std::unordered_map<Node, unsigned int> nodes;
    std::vector<std::optional<Edge>> edges;
    unsigned int reservedSize = 0;
};

template <class Node, class Edge>
Graph<Node, Edge>::Graph(std::vector<Node> nodes)
    : nodes(), edges(std::vector<std::optional<Edge>>(nodes.size() * nodes.size(), std::optional<Edge>())),
      reservedSize(nodes.size()) {
    nodes.reserve(nodes.size());
    unsigned int id = 0;
    for (const auto& node : nodes) {
        nodes.insert({node, id});
        ++id;
    }
}

template <class Node, class Edge>
void Graph<Node, Edge>::clearEdges() {
    std::fill(edges.begin(), edges.end(), std::nullopt);
}

template <class Node, class Edge>
std::size_t Graph<Node, Edge>::edgeId(const Node& n1, const Node& n2) const {
    auto id1 = nodes.find(n1);
    auto id2 = nodes.find(n2);
    if (id1 == nodes.end())
        throw MissingNode<Node>(n1);
    if (id2 == nodes.end())
        throw MissingNode<Node>(n2);

    return id1->second + id2->second * reservedSize;
}

template <class Node, class Edge>
std::optional<Edge> Graph<Node, Edge>::getEdge(const Node& n1, const Node& n2) const {
    return edges[edgeId(n1, n2)];
}

template <class Node, class Edge>
void Graph<Node, Edge>::setEdge(const Node& n1, const Node& n2, const Edge& e) {
    edges[edgeId(n1, n2)] = {e};
}

template <class Node, class Edge>
void Graph<Node, Edge>::clearEdge(const Node& n1, const Node& n2) {
    edges[edgeId(n1, n2)] = {};
}

template <class Node, class Edge>
std::vector<std::pair<const Node&, const std::optional<Edge>&>> Graph<Node, Edge>::getEdges(const Node& n1) const {
    if (nodes.find(n1) == nodes.end())
        throw MissingNode<Node>(n1);

    std::vector<std::pair<const Node&, const std::optional<Edge>&>> e;
    e.reserve(nodes.size() - 1);

    unsigned int nodeId = nodes.at(n1);
    for (const auto& node : nodes) {
        if (node.second == nodeId)
            continue;

        e.push_back({node.first, edges[nodeId + node.second * reservedSize]});
    }
    return e;
}

template <class Node, class Edge>
void Graph<Node, Edge>::insertNode(const Node& n) {
    if (nodes.find(n) != nodes.end())
        return;

    if (!freeIds.empty()) {
        unsigned int id = freeIds.front();
        freeIds.pop();
        nodes.insert({n, id});

        for (const auto& otherNode : nodes) {
            if (otherNode.first == n)
                continue;
            clearEdge(n, otherNode.first);
            clearEdge(otherNode.first, n);
        }
        return;
    }
    nodes.insert({n, static_cast<unsigned int>(nodes.size())});

    std::vector<std::optional<Edge>> newEdges(nodes.size() * nodes.size(), std::optional<Edge>());
    for (std::size_t x = 0; x < nodes.size() - 1; x++) {
        for (std::size_t y = 0; y < nodes.size() - 1; y++) {
            newEdges[x + y * nodes.size()] = edges[x + y * (nodes.size() - 1)];
        }
    }
    edges = std::move(newEdges);
    reservedSize++;
}

template <class Node, class Edge>
void Graph<Node, Edge>::removeNode(const Node& n) {
    auto it = nodes.find(n);
    if (it == nodes.end())
        throw MissingNode<Node>(n);

    freeIds.push(it->second);
    nodes.erase(it);
}

template <class Node, class Edge>
bool Graph<Node, Edge>::hasEdge(const Node& n1, const Node& n2) const {
    std::optional<Edge> e = edges[edgeId(n1, n2)];
    return e.has_value();
}

// MissingNode error

template <class Node>
const char* MissingNode<Node>::what() const noexcept {
    return "requested graph node does not exist";
}

template <class Node>
MissingNode<Node>::MissingNode(Node n) : n(n) {}

template <class Node>
Node MissingNode<Node>::getNode() const {
    return n;
}

} // namespace data

#endif // GAME_GRAPH_H
