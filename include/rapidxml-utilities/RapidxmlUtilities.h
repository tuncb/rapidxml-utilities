#pragma once
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

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
      if (! parent ) return;
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
    if (! parent ) return;
    for (auto node = parent->first_node(nullptr); node != nullptr;
      node = node->next_sibling(nullptr)) {
        fun(node);
    }
  }

namespace detail {

  bool parse_error;

  template <typename T> struct qi_type_generator {};
  template <> struct qi_type_generator<double> {auto type() -> decltype(boost::spirit::qi::double_){return boost::spirit::qi::double_;}};
  template <> struct qi_type_generator<float>  {auto type() -> decltype(boost::spirit::qi::float_) {return boost::spirit::qi::float_;}};
  template <> struct qi_type_generator<int>    {auto type() -> decltype(boost::spirit::qi::int_)   {return boost::spirit::qi::int_;}};
  template <> struct qi_type_generator<size_t> {auto type() -> decltype(boost::spirit::qi::uint_)  {return boost::spirit::qi::uint_;}};

  size_t get_length(const char* str) { return strlen(str); }

  template <typename Ch, typename ParserType> void parse_string(const Ch* str, const ParserType& parser) 
  {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    using ascii::space;

    auto endstr(str + get_length(str)); // get length dows not work with wchar_t
    qi::phrase_parse(str, endstr, parser, space);
    parse_error = (str != endstr);
  }    

  template <typename Ch, typename ParserType, typename OutputType> void parse_string
    (const Ch* str, const ParserType& parser, OutputType& out) 
  {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    using ascii::space;

    auto endstr(str + get_length(str)); // get length dows not work with wchar_t
    qi::phrase_parse(str, endstr, parser, space, out);
    parse_error = (str != endstr);
  }   

  template <typename T, typename Ch> struct from_string_struct {
    T from_string(const Ch* str) 
    {
      using boost::spirit::qi::_1;
      using boost::phoenix::ref;
      T val;

      qi_type_generator<T> gen;
      parse_string(str, gen.type()[ref(val) = _1]);
      return val;
    }
  };

  template <> struct from_string_struct<std::string, char> { std::string from_string(const char* str) {return std::string(str);} };
  template <> struct from_string_struct<std::wstring, wchar_t> { std::wstring from_string(const wchar_t* str) {return std::wstring(str);} };

  template <typename T, typename Ch>
  T from_string(const Ch* str)
  {
    from_string_struct<T, Ch> helper;
    return helper.from_string(str);
  }

  inline void do_nothing() {}
}

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// <summary> Casts attribute value to given type. Throws error if casting fails </summary>
  ///
  /// <typeparam name="T">        Generic type parameter. </typeparam>
  /// <typeparam name="NodeType"> Type of the node type. </typeparam>
  /// <typeparam name="Ch">       Type of the char. </typeparam>
  /// <param name="node">      [in,out] If non-null, the node. </param>
  /// <param name="attr_name"> Name of the attribute. </param>
  ///
  /// <returns> The cast value of attribute. </returns>
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename NodeType, typename Ch> T attribute_cast(NodeType* node, Ch attr_name)
  {
    if (! node) throw std::runtime_error("node does not exists.");
    auto attribute = node->first_attribute(attr_name);
    if (! attribute) throw std::runtime_error("attribute does not exists.");
    T val = detail::from_string<T>(attribute->value());
    if (detail::parse_error) throw std::runtime_error("cast failed."); else return val;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// <summary> Casts attribute value to given type. Return default value if casting fails </summary>
  ///
  /// <typeparam name="T">        Generic type parameter. </typeparam>
  /// <typeparam name="NodeType"> Type of the node type. </typeparam>
  /// <typeparam name="Ch">       Type of the ch. </typeparam>
  /// <param name="node">          [in,out] If non-null, the node. </param>
  /// <param name="attr_name">     Name of the attribute. </param>
  /// <param name="default_value"> The default value. </param>
  ///
  /// <returns> The cast value of attribute. </returns>
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename NodeType, typename Ch> T attribute_cast(NodeType* node, Ch attr_name, T default_value)
  {
    if (! node) return default_value;
    auto attribute = node->first_attribute(attr_name);
    if (! attribute) return default_value;
    T val = detail::from_string<T>(attribute->value());
    if (detail::parse_error) return default_value; else return val;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// <summary>Casts attribute value to given type using the rule given. calss onError on error.</summary>
  ///
  /// <typeparam name="NodeType">  Type of the node type.</typeparam>
  /// <typeparam name="Ch">        Type of the ch.</typeparam>
  /// <typeparam name="ParserType">Type of the parser type.</typeparam>
  /// <param name="node">     [in,out] If non-null, the node.</param>
  /// <param name="attr_name">Name of the attribute.</param>
  /// <param name="rule">   The qi rule used in parsing.</param>
  /// <param name="on_error"> (Optional) function to call if parsing fails.</param>
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  template <typename NodeType, typename Ch, typename RuleType> void attribute_cast_special(
    NodeType* node, Ch attr_name, const RuleType& rule, std::function<void()> on_error = detail::do_nothing)
  {
    if (!node) {on_error(); return;};
    auto attribute = node->first_attribute(attr_name);
    if (! attribute) {on_error(); return;};

    detail::parse_string(attribute->value(), rule);
    if (detail::parse_error) on_error;
  }

  //template <typename VectorType, typename NodeType, typename Ch, typename SeperatorType> void attribute_cast_pushback(
  //  NodeType* node, Ch attr_name, VectorType& vec, SeperatorType seperator = boost::spirit::ascii::space
  //  , std::function<void()> on_error = detail::do_nothing)
  //{
  //  if (!node) {on_error(); return;};
  //  auto attribute = node->first_attribute(attr_name);
  //  if (! attribute) {on_error(); return;};



  //  detail::parse_string(attribute->value(), rule);
  //  if (detail::parse_error) on_error;
  //}


}