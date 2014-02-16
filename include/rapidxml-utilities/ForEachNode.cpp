#include <boost/test/unit_test.hpp>
#include <rapidxml/rapidxml.hpp>
#include <BulkFileReader/BulkFileReader.h>
#include <rapidxml-utilities/ForEachNode.h>

struct FixtureForEachNode {
  FixtureForEachNode() 
  {
    input = ozp::bulk_read_file("test1.xml");
    doc.parse<0>(input.get());
    top_node = doc.first_node("rapidxmlutilities");
  }

  rapidxml::xml_node<>* top_node;
  rapidxml::xml_document<> doc;
  std::unique_ptr<char []> input;
};


BOOST_FIXTURE_TEST_SUITE (ForEachNode, FixtureForEachNode)

BOOST_AUTO_TEST_CASE(for_each_node_all) {
  size_t count = 0;
  rapidxml::for_each_node(top_node, [&](rapidxml::xml_node<>*){
    count++;
  });
  BOOST_CHECK_EQUAL(count, 5);
}

BOOST_AUTO_TEST_CASE(for_each_node_specific) {
  size_t count = 0;
  rapidxml::for_each_node(top_node, "test1", [&](rapidxml::xml_node<>*){
    count++;
  });
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_SUITE_END()