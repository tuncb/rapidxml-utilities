#include <fstream>
#include <rapidxml\rapidxml_print.hpp>
#include "XmlBuilder.h"

void ozp::XmlNode::save(const std::string& filename)
{
  using namespace rapidxml;
  using namespace std;

  xml_document<> doc;

  auto my_node = doc.allocate_node(node_element, this->name.c_str());
  doc.append_node(my_node);

  for (auto&& attr_pair : this->attributes) {
    auto attr = doc.allocate_attribute(attr_pair.first.c_str(), attr_pair.second.c_str());
    my_node->append_attribute(attr);
  }
  
  for(auto&& subnode : this->nodes) {
    subnode.to_node(doc, my_node);
  }

  ofstream file;
  file.open(filename);
  file << doc;
  file.close();
}


void ozp::XmlNode::to_node(rapidxml::xml_document<>& doc, rapidxml::xml_node<char>* node)
{
  using namespace rapidxml;
  using namespace std;
  
  auto my_node = doc.allocate_node(node_element, this->name.c_str());
  node->append_node(my_node);

  for (auto&& attr_pair : this->attributes) {
    auto attr = doc.allocate_attribute(attr_pair.first.c_str(), attr_pair.second.c_str());
    my_node->append_attribute(attr);
  }

  for(auto&& subnode : this->nodes) {
    subnode.to_node(doc, my_node);
  }
}