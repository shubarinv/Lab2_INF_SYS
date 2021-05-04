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
  matrix<Node>* grid;
  Node* startNode{nullptr};
  Node* endNode{nullptr};
  char status;
  bool pathFound{false};

  static bool CompareScore(Node* lhs, Node* rhs) {
	return lhs->GetF() < rhs->GetF();
  }

 public:
  [[nodiscard]] char GetStatus() const {
	return status;
  }
  [[maybe_unused]] void FindPath() {
	if (!IsAccessible(startNode)) {
	  status = 'i';
	  spdlog::info("Start node blocked");
	  return;
	}
	spdlog::info("Start node is ok");
	if (!IsAccessible(endNode)) {
	  status = 'b';
	  spdlog::info("End Node blocked");
	  return;
	}
	spdlog::info("End node is ok");
	openList.clear();
	closedList.clear();

	openList.push_back(startNode);
	while (!openList.empty()) {
	  Node* currentNode = (*std::min_element(openList.begin(), openList.end(), CompareScore));
	  (void)openList.remove(currentNode);
	  closedList.push_back(currentNode);
	  if ((std::find(closedList.begin(), closedList.end(), endNode) != closedList.end())) {
		break;
	  }
	  checkAdjacentNodes(*currentNode);
	  if (pathFound) {
		break;
	  }
	}
	if (!pathFound) {
	  status = 'f';
	}
  }
  void reconstructPath(Node* node) {
	if (node == nullptr) return;
	if (node->GetParent() == nullptr) {
	  return;
	}
	if (node != startNode && node != endNode)
	  node->SetType(3);
	if(node->GetType()==-1){
	  spdlog::error("Excuse me what the hell???");
	}
	reconstructPath(node->GetParent());
  }

  [[maybe_unused]] void SetGrid(matrix<Node>* newGrid) {
	grid = newGrid;
  }
  void aStar() {
	spdlog::info("=======================");
	status = ' ';
	startNode = nullptr;
	endNode = nullptr;
	pathFound = false;
	spdlog::info("Looking for start and end nodes...");
	for (int row = 0; row < grid->rows(); ++row) {
	  for (int column = 0; column < grid->columns(); ++column) {
		auto tmp = grid->get(row, column);
		if (tmp.GetType() == 1) {
		  spdlog::info("Start node found! y: {} x:{}", row, column);
		  startNode = &grid->get(row, column);
		  startNode->SetG(0);
		  startNode->SetH(0);
		  startNode->SetF(0);
		}
		if (grid->get(row, column).GetType() == 2) {
		  spdlog::info("End node found! y: {} x:{}", row, column);
		  endNode = &grid->get(row, column);
		}
	  }
	}
	if (startNode == nullptr) {
	  status = 's';
	  return;
	}
	if (endNode == nullptr) {
	  status = 'e';
	  return;
	}
	FindPath();
  }

  bool isValid(vec2i position) {
	return position.first >= 0 && position.first <= grid->rows() - 1 && position.second >= 0 && position.second <= grid->columns() - 1;
  }

  bool isDestination(Node* node) {
	return node == endNode;
  }

  Node* getIFValid(vec2i position) {
	if (isValid(position)) {
	  return &grid->get(position.first, position.second);
	}
	return nullptr;
  }

  void checkAdjacentNodes(Node& node) {
	/**
     * 0 - current node
     *   a--b--c
     *   |  |  |
     *   d--0--e
     *   |  |  |
     *   f--g--h
    */
	//if (!pathFound) checkNode(getIFValid({node.position.first - 1, node.position.second - 1}), &node);//a
	if (!pathFound) checkNode(getIFValid({node.position.first - 1, node.position.second}), &node);    //b
//	if (!pathFound) checkNode(getIFValid({node.position.first - 1, node.position.second + 1}), &node);//c

	if (!pathFound) checkNode(getIFValid({node.position.first, node.position.second - 1}), &node);    //d
	if (!pathFound) checkNode(getIFValid({node.position.first, node.position.second + 1}), &node);    //e

	//if (!pathFound) checkNode(getIFValid({node.position.first + 1, node.position.second - 1}), &node);//f
	if (!pathFound) checkNode(getIFValid({node.position.first + 1, node.position.second}), &node);    //g
	//if (!pathFound) checkNode(getIFValid({node.position.first + 1, node.position.second + 1}), &node);//h
  }
  void checkNode(Node* node, Node* parent) {
	if (node == nullptr) return;
	if (node->GetType() == -1) return;

	if (isDestination(node)) {
	  pathFound = true;
	  node->UpdateParent(parent);
	  reconstructPath(node);
	  return;
	} else if (std::find(closedList.begin(), closedList.end(), node) == closedList.end()) {// not in closedList
	  double h, g, tentativeScore;
	  g = parent->GetG() + 1.0;
	  h = Node::CalculateH(node->position, endNode->position);
	  tentativeScore = Node::ComputeScore(g, h);

	  if (std::find(openList.begin(), openList.end(), node) == openList.end()) {// not in openList
		node->SetG(g);
		node->SetF(tentativeScore);
		node->SetH(h);
		node->UpdateParent(parent);
		openList.push_back(node);
	  } else {
		if (tentativeScore < node->GetF()) {
		  node->SetG(g);
		  node->SetF(tentativeScore);
		  node->SetH(h);
		  node->UpdateParent(parent);
		  openList.push_back(node);
		}
	  }
	}
  }
  void ResetVisitedNodes() {
	for (int row = 0; row < grid->rows(); ++row) {
	  for (int column = 0; column < grid->columns(); ++column) {
		if (grid->get(row, column).GetType() == 3) {
		  grid->get(row, column).SetType(0);
		}
	  }
	}
  }
  bool IsAccessible(Node* node) {
	auto adjNodes = getAdjacentNodes(node);
	int blockedNodes = 0;
	for (auto& adjNode : adjNodes) {
	  if (adjNode->GetType() == -1) {
		blockedNodes++;
	  }
	}
	if (blockedNodes < adjNodes.size()) {
	  return true;
	}
	return false;
  }
  std::vector<Node*> getAdjacentNodes(Node* node) {
	std::vector<Node*> tmp;
	if (getIFValid({node->position.first - 1, node->position.second - 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first - 1, node->position.second - 1}));
	}
	if (getIFValid({node->position.first - 1, node->position.second}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first - 1, node->position.second}));
	}
	if (getIFValid({node->position.first - 1, node->position.second + 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first - 1, node->position.second + 1}));
	}
	if (getIFValid({node->position.first, node->position.second - 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first, node->position.second - 1}));
	}
	if (getIFValid({node->position.first, node->position.second + 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first, node->position.second + 1}));
	}
	if (getIFValid({node->position.first + 1, node->position.second - 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first + 1, node->position.second - 1}));
	}
	if (getIFValid({node->position.first + 1, node->position.second}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first + 1, node->position.second}));
	}
	if (getIFValid({node->position.first + 1, node->position.second + 1}) != nullptr) {
	  tmp.push_back(getIFValid({node->position.first + 1, node->position.second + 1}));
	}
	spdlog::info("for node({}:{}) there are {} adjacent nodes",node->position.first,node->position.second,tmp.size());

	return tmp;
  }

  Node* getStartNode() {
	return startNode;
  }
  Node* getEndNode() {
	return endNode;
  }
};