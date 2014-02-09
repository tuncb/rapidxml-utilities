#pragma once
#include <string>

namespace rapidxml {

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// <summary> Execute lambda for each child node matching name. </summary>
  ///
  /// <typeparam name="NodeType">   Type of the node type. </typeparam>
  /// <typeparam name="Ch">         Type of the character rapidxml doc uses. </typeparam>
  /// <typeparam name="LambdaType"> Type of the lambda type. </typeparam>
  /// <param name="parent">     [in,out] Parent node. </param>
  /// <param name="child_name"> Name of the child node. </param>
  /// <param name="fun">        lambda function called for each matching child node. </param>
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  template <typename NodeType, typename Ch, typename LambdaType> inline void for_each_node(NodeType* parent, const Ch* child_name,
    LambdaType fun) {
      for (auto node = parent->first_node(child_name); node != nullptr;
        node = node->next_sibling(child_name)) {
          fun(node);
      }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// <summary> Execute lambda for each child node. </summary>
  ///
  /// <typeparam name="NodeType">   Type of the node type. </typeparam>
  /// <typeparam name="LambdaType"> Type of the lambda type. </typeparam>
  /// <param name="parent">     [in,out] Parent node. </param>
  /// <param name="fun">        lambda function called for each matching child node. </param>
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  template <typename NodeType, typename LambdaType> inline void for_each_node(NodeType* parent, LambdaType fun) {
    for (auto node = parent->first_node(nullptr); node != nullptr;
      node = node->next_sibling(nullptr)) {
        fun(node);
    }
  }

  //template <typename NodeType, typename T>
  //T extract_attribute(NodeType* node, char* 



}
