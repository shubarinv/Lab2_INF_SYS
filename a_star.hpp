#include <fstream>
#include <list>
#include <sstream>
#include <vector>

#include "matrix.hpp"
#include "node.hpp"
class A_Star {
 private:
  std::list<Node*> openList;  ///<  nodes that have been visited but not expanded (meaning that successors have not been explored yet). This is the list of pending tasks.
  std::list<Node*> closedList;///<  nodes that have been visited and expanded (successors have been explored already and included in the openList list, if this was the case).
 matrix<int> grid;
  Node startNode;
  Node endNode;

  void expandNode(Node* node) {
	for (auto& adjNode : node->GetAdjacentNodes()) {
	  if ((std::find(closedList.begin(), closedList.end(), &adjNode) != closedList.end())) {
		continue;
	  } else if ((std::find(openList.begin(), openList.end(), &adjNode) != openList.end())) {
		if (adjNode.ComputeScore(*node, endNode) < adjNode.GetF()) {
		  adjNode.SetF(adjNode.ComputeScore(*node, endNode));
		  adjNode.UpdateParent(node);
		}
	  } else {
		adjNode.SetF(adjNode.ComputeScore(*node, endNode));
		openList.push_back(&adjNode);
	  }
	}
  }
  static bool CompareScore(Node* lhs, Node* rhs) {
	return lhs->GetF() < rhs->GetF();
  }

 public:
  [[maybe_unused]] void FindPath(vec2i start, vec2i goal) {
	openList.clear();
	closedList.clear();

	startNode.position = start;

	endNode.position = goal;

	openList.push_back(&startNode);
	while (!openList.empty()) {
	  Node* currentNode = (*std::min_element(openList.begin(), openList.end(), CompareScore));

	  (void)openList.remove(currentNode);
	  closedList.push_back(currentNode);
	  if ((std::find(closedList.begin(), closedList.end(), &endNode) != closedList.end())) {
		break;
	  }
	  expandNode(currentNode);
	}
  }
  [[maybe_unused]] void SetGrid(matrix<int>& newGrid) {
	grid = newGrid;
  }

};