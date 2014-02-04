#pragma once
#include <string>
#include <array>

namespace std {

  template <typename Vec>
  string to_string(const Vec& vec) 
  {
    string str;
    for (size_t i = 0; i < vec.size(); ++i) {
      str += to_string(vec[i]) + ",";
    }
    str.pop_back();
    return str;
  }

  string to_string(const std::array<bool, 6>& vec) 
  {
    string str;
    for (size_t i = 0; i < 6; ++i) {
      str += to_string(int(vec[i]));
    }
    return str;
  }



}