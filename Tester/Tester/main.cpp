#define BOOST_TEST_MODULE RapidXmlUtilities

#include <boost/test/unit_test.hpp>
#include <string>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml-utilities/RapidxmlUtilities.h>
#include <BulkFileReader/BulkFileReader.h>

struct FixtureXml1 {
  FixtureXml1() 
  {
    input = ozp::bulk_read_file("test1.xml");
    doc.parse<0>(input.get());
    top_node = doc.first_node("rapidxmlutilities");
  }

  std::unique_ptr<char []> input;
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* top_node;
};


BOOST_FIXTURE_TEST_SUITE (xml1, FixtureXml1)

BOOST_AUTO_TEST_CASE(for_each_node_all) {
  size_t count = 0;
  rapidxml::for_each_node(top_node, [&](rapidxml::xml_node<>* node){
    count++;
  });
  BOOST_CHECK_EQUAL(count, 5);
}

BOOST_AUTO_TEST_CASE(for_each_node_specific) {
  size_t count = 0;
  rapidxml::for_each_node(top_node, "test1", [&](rapidxml::xml_node<>* node){
    count++;
  });
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(attribute_cast_string) {
  auto node = top_node->first_node("test");
  auto str = rapidxml::attribute_cast<std::string>(node, "str");
  BOOST_CHECK_EQUAL(str, "test");
}

BOOST_AUTO_TEST_CASE(attribute_cast_string_default_nonode) {
  auto node = top_node->first_node("err_node");
  auto str = rapidxml::attribute_cast<std::string>(node, "str", "test");
  BOOST_CHECK_EQUAL(str, "test");
}

BOOST_AUTO_TEST_CASE(attribute_cast_double) {
  auto node = top_node->first_node("test");
  auto value = rapidxml::attribute_cast<double>(node, "double");
  BOOST_CHECK_EQUAL(value, 1.0);
}

BOOST_AUTO_TEST_CASE(attribute_cast_double_error) {
  auto node = top_node->first_node("test");
  auto value = rapidxml::attribute_cast<double>(node, "str", 1.0);
  BOOST_CHECK_EQUAL(value, 1.0);
}

BOOST_AUTO_TEST_SUITE_END()