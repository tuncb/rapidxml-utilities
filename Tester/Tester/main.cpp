#define BOOST_TEST_MODULE RapidXmlUtilities

#include <boost/test/unit_test.hpp>
#include <string>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml-utilities/RapidxmlUtilities.h>
#include <BulkFileReader/BulkFileReader.h>


BOOST_AUTO_TEST_CASE(dummy) {
  BOOST_CHECK_EQUAL(true, true);
}

BOOST_AUTO_TEST_CASE(test1) {
  auto input = ozp::bulk_read_file("test1.xml");
  rapidxml::xml_document<> doc;
  doc.parse<0>(input.get());

  auto top_node = doc.first_node("rapidxmlutilities");

  size_t count = 0;
  rapidxml::for_each_node(top_node, [&](rapidxml::xml_node<>* node){
    auto str = rapidxml::attribute_cast<std::string>(node, "str");
    count++;
  });

  BOOST_CHECK_EQUAL(count, 5);
}