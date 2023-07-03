#ifndef PTREEPRINTER_HPP
 #define  PTREEPRINTER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;


class PTreePrinter
{
    public:
        static void printStringToJson(const std::string& jsonStr);
        static void printJsonToCsv(const std::string& jsonStr, const std::vector<std::string>& keys);
};

 #endif