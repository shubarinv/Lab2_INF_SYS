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
	  f{std::numeric_limits<double>::max()},        ///< sum of g and h
	  g{std::numeric_limits<double>::max()},      ///< cost of moving from initial cell to current cell (sum of all visited cells)
	  h{std::numeric_limits<double>::max()};      ///< estimated cost for moving from current cell to final cell
  short type = {0}; /**
 * -1 -wall
 * 0 -walkable area
 * 1 -start area
 * 2 -target area
 * 3 -visited area
 * */

 public:
  vec2i position{-1, -1};

  static double ComputeScore(Node& currentNode, Node& goal) {
	return CalculateH(currentNode.position, goal.position) + currentNode.g + 1.0;
  }
  static double ComputeScore(double t_g,double t_h){
	return t_g + t_h;
  }
  static double CalculateH(vec2i src, vec2i dest) {
	// h is estimated with the two points distance formula
	return sqrt(pow((src.first - dest.first), 2.0)
				+ pow((src.second - dest.second), 2.0));
  }
  [[nodiscard]] double GetF() const {
	return f;
  }
  [[nodiscard]] double GetG() const{
	return g;
  }
  [[nodiscard]] double GetH() const{
	return h;
  }
  void SetF(double t_f) {
	f = t_f;
  }
  void SetG(double t_g) {
	g = t_g;
  }
  void SetH(double t_h) {
	h = t_h;
  }
  void UpdateParent(Node* newParent) {
	parent = newParent;
  }
  [[nodiscard]] short GetType() const {
	return type;
  }
  void SetType(short t_type) {
	type = t_type;
  }
  Node* GetParent() {
	return parent;
  }
};
#endif//ANTALG__NODE_HPP_
