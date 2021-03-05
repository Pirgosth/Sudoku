#include <algorithm>
#include <set>
#include <iostream>

class PossibilityNode;

bool comparePossibilities(const PossibilityNode *p1, const PossibilityNode *p2);

typedef std::set<PossibilityNode *, decltype(&comparePossibilities)>::iterator PossibilityNodeIterator;

class PossibilityNode
{
private:
  const int m_value;
  const int m_selfRemainingValues;
  bool m_locked;
  std::set<PossibilityNode *, decltype(&comparePossibilities)> m_nextPossibilities;

public:
  PossibilityNode(int value, int selfRemainingValues = 9);
  ~PossibilityNode();

  int getValue() const;
  bool isValid();
  bool isLocked();

  std::set<PossibilityNode *, decltype(&comparePossibilities)> &getNextPossibilities();

  void addPossibility(int value);
  void removePossibility(int value);
};