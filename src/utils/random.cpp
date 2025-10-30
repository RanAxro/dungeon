#include "include/random.hpp"

template<typename T>
T random(T min, T max){
  static std::random_device rd;  
  static std::mt19937 gen(rd()); 
  if constexpr (std::is_integral<T>::value){
    std::uniform_int_distribution<T> distrib(min, max);
    return distrib(gen);
  }else if constexpr (std::is_floating_point<T>::value){
    std::uniform_real_distribution<T> distrib(min, max);
    return distrib(gen);
  }
}