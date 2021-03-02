#include "random.hpp"

void initRand(){
  std::random_device rd;
  mt = new std::mt19937(rd());
}

void destroyRand(){
  delete mt;
}

int randRange(int a, int b){
  std::uniform_int_distribution<int> dist(a, b);
  if(mt != nullptr){
    return dist(*mt);
  }
  else{
    return 0;
  }
}
