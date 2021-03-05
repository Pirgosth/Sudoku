#include "possibility_node.hpp"

PossibilityNode::PossibilityNode(int value, int selfRemainingValues) : m_value(value), m_selfRemainingValues(selfRemainingValues), m_locked(false), m_nextPossibilities(&comparePossibilities) {}

PossibilityNode::~PossibilityNode()
{
  for (auto it = m_nextPossibilities.begin(); it != m_nextPossibilities.end(); it++)
  {
    PossibilityNode *possibility = *it;
    if (possibility != __null)
      delete possibility;
  }
}

int PossibilityNode::getValue() const
{
  return m_value;
}

bool PossibilityNode::isValid()
{
  return !m_nextPossibilities.empty();
}

bool PossibilityNode::isLocked()
{
  return m_locked;
}

std::set<PossibilityNode *, decltype(&comparePossibilities)> &PossibilityNode::getNextPossibilities()
{
  return m_nextPossibilities;
}

void PossibilityNode::addPossibility(int value)
{
  m_nextPossibilities.insert(new PossibilityNode(value));
}

void PossibilityNode::removePossibility(int value)
{
  PossibilityNodeIterator possibilityIterator = std::find_if(m_nextPossibilities.begin(), m_nextPossibilities.end(), [value](const PossibilityNode *node) -> bool { return node->getValue() == value; });

  //If not found, nothing to remove
  if (possibilityIterator == m_nextPossibilities.end())
  {
    return;
  }

  m_nextPossibilities.erase(*possibilityIterator);

  if (m_nextPossibilities.size() == 0)
  {
    m_locked = true;
  }
}

bool comparePossibilities(const PossibilityNode *p1, const PossibilityNode *p2)
{
  return p1->getValue() < p2->getValue();
}