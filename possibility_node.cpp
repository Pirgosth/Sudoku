#include "possibility_node.hpp"

template<int N>
PossibilityNode<N>::PossibilityNode(bool isValid, int count){
  for(auto node = m_nodes.begin(); node!=m_nodes.end(); node++){
    *node = 0;
  }
  m_isValid = isValid;
  m_count = count;
}

template<int N>
PossibilityNode<N>::~PossibilityNode(){
  for(auto node = m_nodes.begin(); node!=m_nodes.end(); node++){
    PossibilityNode* child = *node;
    if(child != 0)
      delete child;
  }
}

template<int N>
PossibilityNode<N>*& PossibilityNode<N>::operator[](int index){
  return m_nodes[index];
}

template<int N>
bool PossibilityNode<N>::isValid(){
  return m_isValid;
}

template<int N>
void PossibilityNode<N>::setValidity(bool validity){
  m_isValid = validity;
  // if(!validity){
  //     for(auto node = m_nodes.begin(); node != m_nodes.end(); node++){
  //         delete (*node);
  //         (*node) = 0;
  //     }
  // }
}

template<int N>
int PossibilityNode<N>::getCount(){
  return m_count;
}

template class PossibilityNode<9>;