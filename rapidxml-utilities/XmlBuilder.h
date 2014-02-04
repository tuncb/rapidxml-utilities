#pragma once
#include <map>
#include <vector>
#include <string>
#include <rapidxml/rapidxml.hpp>

namespace ozp {
  class XmlNode
  {
  public:
    XmlNode(std::string node_name) : name(std::move(node_name)) {} 

    std::string name;
    std::map<std::string, std::string> attributes;
    std::vector<XmlNode> nodes;

    void save(const std::string& filename);
  private:
    void to_node(rapidxml::xml_document<>& doc, rapidxml::xml_node<char>* node);
  };

}