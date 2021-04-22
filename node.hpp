//
// Created by Vladimir on 21.04.2021.
//

#ifndef ANTALG__NODE_HPP_
#define ANTALG__NODE_HPP_
#include <cmath>
typedef std::pair<int, int> vec2i;
class Node {
   Node* parent{nullptr};
  double
	  f{-1.0},///< sum of g and h
	  g{-1.0},///< cost of moving from initial cell to current cell (sum of all visited cells)
	  h{-1.0};///< estimated cost for moving from current cell to final cell
  std::list<Node> adjacentNodes;

 public:
  vec2i position{-1, -1};
  std::list<Node>& GetAdjacentNodes() {
	return adjacentNodes;
  }
  [[maybe_unused]] void AddAdjacentNode(const Node& newNode) {
	if (adjacentNodes.size() >= 4) {
	  std::cerr << "Node::addAdjacentNode there are already 4 adjacent nodes!";
	  throw std::runtime_error("Error in Node::addAdjacentNode too many adjacent nodes");
	}

	adjacentNodes.push_back(newNode);
	adjacentNodes.back().parent = this;
  }
  double ComputeScore(Node& currentNode, Node& goal) {
	g = currentNode.g + 1.0;
	h = static_cast<double>(abs(goal.position.first - this->position.first) + abs(goal.position.second - this->position.second));
	return h + g;
  }
  [[nodiscard]] double GetF() const {
	return f;
  }
  void SetF(double t_f) {
	f = t_f;
  }
  void UpdateParent(Node*newParent) {
	parent=newParent;
  }
};
#endif//ANTALG__NODE_HPP_
