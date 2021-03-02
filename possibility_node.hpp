#include <array>

template<int N>
class PossibilityNode{
private:
  std::array<PossibilityNode*, N> m_nodes;
  bool m_isValid = true;
  int m_count;
public:
  PossibilityNode(bool isValid, int count);
  ~PossibilityNode();
  PossibilityNode*& operator[](int index);
  bool isValid();
  void setValidity(bool validity);
  int getCount();
};