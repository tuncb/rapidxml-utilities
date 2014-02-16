rapidxml-utilities
========

A baunch of utility functions to use with rapidxml library.

License
--------
Boost license.

Prerequests
---------------
* rapidxml library
* attribute_cast functions are based on boost spirit library.
* tests are based on Boost.Test

Installation
----------
Header only
~~~~cpp
#include <rapidxml-utilities/rapidxml-utilities.h>
~~~~
or parts of the library can be included seperately
~~~~cpp
#include <rapidxml-utilities/ForEachNode.h> // for_each_node
#include <rapidxml-utilities/AttributeCast.h> // attribute_cast
#include <rapidxml-utilities/Document.h> // add_node, add_attribute etc...
~~~~

Documentation
--------------
### ForEachNode
~~~~cpp
#include <rapidxml-utilities/ForEachNode.h>
// call lambda for each child node
rapidxml::for_each_node(parent_node, [](rapidxml::xml_node<>* node){});
// call lambda for each child node with name
rapidxml::for_each_node(parent_node, name, [](rapidxml::xml_node<>* node){});
~~~~
### AttributeCast
~~~~cpp
#include <rapidxml-utilities/AttributeCast.h>
~~~~

