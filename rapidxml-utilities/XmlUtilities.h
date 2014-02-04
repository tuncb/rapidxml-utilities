// This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/ or
// send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
// Panthalassa by Tunç Bahçecioğlu is licensed under a Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License.
// Permissions beyond the scope of this license may be available at www.tuncbahcecioglu.com.

#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <array>
#include <RapidXml\rapidxml.hpp>
#include "FromString.h"

namespace rapidxml {

template <typename NodeType, typename Ch, typename LambdaType>
inline void for_each_node(NodeType* parent, const Ch* child_name,
                          LambdaType fun) {
    for (auto node = parent->first_node(child_name); node != nullptr;
         node = node->next_sibling(child_name)) {
        fun(node);
    }
}
template <typename NodeType, typename LambdaType>
inline void for_each_node(NodeType* parent, LambdaType fun) {
    for (auto node = parent->first_node(nullptr); node != nullptr;
         node = node->next_sibling(nullptr)) {
        fun(node);
    }
}

class BadAttribute : public std::runtime_error
{
public:
	BadAttribute() : std::runtime_error("") {}
	BadAttribute(const std::string& nodename, const std::string& attributename) 
		: std::runtime_error("Error parsing Xml attribute : " + attributename + " of Xml node: " + nodename), nodename(nodename), attributename(attributename)
	{}
	public:
		std::string nodename;
		std::string attributename;
};

class BadElement : public std::runtime_error
{
public:
	BadElement() : std::runtime_error("") {}
	BadElement(const std::string& nodename) 
		: std::runtime_error("Error parsing Xml element : " + nodename), nodename(nodename)
	{}
	public:
		std::string nodename;
};

template <typename T>
inline T getXmlAttribute(const rapidxml::xml_node<>* node, const char* attribute_name)
{
	T ret;
	rapidxml::xml_attribute<>* attribute = node->first_attribute(attribute_name);
	if (attribute) 
	{
		try 
		{
			ret = ptl::fromString<T>(attribute->value());
		}
		catch (...)
		{
			throw BadAttribute(node->name(),attribute_name);
		}
	}
	else 
		throw BadAttribute(node->name(),attribute_name);
	return ret;
}

template<typename T>
void getXmlVectorAttribute(const rapidxml::xml_node<>* node, const char* attribute_name, std::vector<T>& result)
{
	rapidxml::xml_attribute<>* attribute = node->first_attribute(attribute_name);
	if (attribute) {
		try {
			ptl::fromString(attribute->value(), result);
		}
		catch (const boost::bad_lexical_cast&) {
			throw BadAttribute(node->name(),attribute_name);
		}
	} else throw BadAttribute(node->name(),attribute_name);
}

template<typename T, size_t NUM>
void getXmlVectorAttribute(const rapidxml::xml_node<>* node, const char* attribute_name, std::array<T, NUM>& result)
{
	rapidxml::xml_attribute<>* attribute = node->first_attribute(attribute_name);
	if (attribute) {
		try {
			ptl::fromString(attribute->value(), result);
		} catch (const boost::bad_lexical_cast&) {
			throw BadAttribute(node->name(),attribute_name);
		}
	} else throw BadAttribute(node->name(),attribute_name);
}

template<typename T>
inline T getXmlAttribute(const rapidxml::xml_node<>* node, const char* attribute_name, T default_value)
{
	rapidxml::xml_attribute<>* attribute = node->first_attribute(attribute_name);
  if (attribute) {
		try {
			return ptl::fromString(attribute->value(), default_value);
		} catch(const boost::bad_lexical_cast&) { 
      return default_value;
		}
  } else {
    return default_value;
  }
}

inline void getPropertyMap(const rapidxml::xml_node<>* node, std::map<std::string, std::string>& properties)
{
	auto attribute = node->first_attribute();
	while(attribute)
	{
		properties[attribute->name()] = attribute->value();
		attribute = attribute->next_attribute();
	}
}

}
