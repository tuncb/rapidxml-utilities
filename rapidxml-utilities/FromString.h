#pragma once

#include <boost\lexical_cast.hpp>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <boost\algorithm\string.hpp>
#include <boost/spirit/include/qi.hpp>

namespace ptl {


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Converts a string to an arbitrary type. </summary>
/// <remarks>	uses boost::lexical_cast except bool and std::bitset<6> types. 
/// 			Throws boost::bad_lexical_cast in case of error.   </remarks>
/// <param name="str">	The string input, overloads for const char* and const std::string& </param>
/// <returns>	converted object </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline T fromString(const char* str)
{
	return boost::lexical_cast<T>(str);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Converts a string to an arbitrary type. </summary>
/// <remarks>	uses boost::lexical_cast except bool and std::bitset<6> types. 
/// 			Throws boost::bad_lexical_cast in case of error.   </remarks>
/// <param name="str">	The string input, overloads for const char* and const std::string& </param>
/// <returns>	converted object </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline T fromString(const std::string& str)
{
	return ptl::fromString<T>(str.c_str());
}

template <>
inline const char* fromString(const char* str)
{
	return str;
}

template<>
inline bool fromString(const char* str)
{
	if (strncmp(str, "true", 4) == 0) return true;
	if (strncmp(str, "false", 5) == 0)return false;

	throw boost::bad_lexical_cast();
}

template<>
inline std::array<bool, 6> fromString(const char* str)
{
	std::array<bool, 6> bits;

  for (size_t i = 0; i < 6; ++i) {
    if (str[i] == '0') { bits[i] = false; } 
    else if (str[i] == '1') { bits[i] = true; } 
    else {throw boost::bad_lexical_cast();}
  }

	return bits;
}

template <typename T>
inline T fromString(const char* str, T default_value)
{
	try
	{
		T val = fromString<T>(str);
		return val;
	}
	catch (const boost::bad_lexical_cast&)
	{
		return default_value;
	}
}

template <typename T>
inline T fromString(const std::string& str, T default_value)
{
	return fromString(str.c_str(), default_value);
}

/* specialized vector parsing */

namespace detail {
  template <typename T> struct qi_type_generator {};
  template <> struct qi_type_generator<double> {auto type() -> decltype(boost::spirit::qi::double_){return boost::spirit::qi::double_;}};
  template <> struct qi_type_generator<float>  {auto type() -> decltype(boost::spirit::qi::float_){return boost::spirit::qi::float_;}};
  template <> struct qi_type_generator<int>    {auto type() -> decltype(boost::spirit::qi::int_){return boost::spirit::qi::int_;}};
  template <> struct qi_type_generator<size_t> {auto type() -> decltype(boost::spirit::qi::uint_){return boost::spirit::qi::uint_;}};

}

template <typename T>
inline void fromString(const char* str, std::vector<T>& vec)
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  const char* endstr(str + strlen(str));
  detail::qi_type_generator<T> qi_gen;

  qi::phrase_parse(str, endstr, qi_gen.type() % ',',ascii::space, vec);
}

template <typename T, size_t NUM>
inline void fromString(const char* str, std::array<T, NUM>& vec)
{
	std::vector<std::string> items;
	boost::split(items, str, boost::is_any_of(","));
  size_t c = 0;
	for(auto s_iter = std::begin(items); s_iter != std::end(items); ++s_iter)
	{
		boost::trim(*s_iter);
		vec[c++] = fromString<T>(*s_iter);
	}
}

/* */

template <typename T, size_t NUM>
inline void fromString(const std::string& str, std::array<T, NUM>& vec)
{
  fromString(str.c_str(), vec);
}

template <typename T>
inline void fromString(const std::string& str, std::vector<T>& vec)
{
	fromString(str.c_str(), vec);
}

template<typename T>
inline T fromPropertyMap(const std::map<std::string, std::string>& properties, const std::string& property_name)
{
	auto iter = properties.find(property_name);
	if (iter != properties.end())
	{
		return fromString<T>(iter->second);
	}
	else
		throw boost::bad_lexical_cast();
}

template<typename T>
inline T fromPropertyMap(const std::map<std::string, std::string>& properties, const std::string& property_name, T default_value)
{
	auto iter = properties.find(property_name);
	if (iter != properties.end())
	{
		try 
		{
			return fromString(iter->second, default_value);
		}
		catch(const boost::bad_lexical_cast&)
		{
			return default_value;
		}
	}
	else
		return default_value;
}

template<typename T>
inline void fromPropertyMap(const std::map<std::string, std::string>& properties, const std::string& property_name, std::vector<T>& vec)
{
	auto iter = properties.find(property_name);
	if (iter != properties.end())
	{
		fromString(iter->second, vec);
	}
}

}