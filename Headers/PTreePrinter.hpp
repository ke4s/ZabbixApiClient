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

    static void printStringToJson(const std::string& jsonStr, const std::vector<std::string>& keys)
    {
        std::istringstream is(jsonStr);
        pt::ptree jsonResponse;
        pt::read_json(is, jsonResponse);
        /*
        const auto& results = jsonResponse.get_child("result");

        pt::ptree jsonOutput;
        pt::ptree nullTree;
        for (const auto& kv : results) {
            const auto& obj = kv.second;

            pt::ptree jsonRow;

            for (const auto& key : keys) {
                std::vector<std::string> keyParts;
                boost::split(keyParts, key, boost::is_any_of("."));

                const pt::ptree* currentNode = &obj;
                for (const auto& part : keyParts) {
                    currentNode = &currentNode->get_child(part, nullTree);
                    if (currentNode == &nullTree)
                        break;
                }

                if (currentNode != &nullTree)
                    jsonRow.put(key, currentNode->get_value<std::string>());
                else
                     jsonRow.put(key,"N/A");
            }
            jsonOutput.push_back(std::make_pair("", jsonRow));
        }
        */
        pt::write_json(std::cout, jsonResponse);
    }



    static void printJsonToCsv(const std::string& jsonStr, const std::vector<std::string>& keys)
    {
        std::istringstream is(jsonStr);
        pt::ptree jsonResponse;
        pt::read_json(is, jsonResponse);

        const auto& results = jsonResponse.get_child("result");

        std::cout << boost::algorithm::join(keys, ",") << "\n";

        pt::ptree nullTree;
        for (const auto& kv : results) {
            auto& obj = kv.second;

            std::vector<std::string> csvRow;
            for (const auto& key : keys) {

                std::vector<std::string> keyParts;
                boost::split(keyParts, key, boost::is_any_of("."));

                const pt::ptree* currentNode = &obj;
                for (const auto& part : keyParts)
                {
                    currentNode = &currentNode->get_child(part, nullTree);
                    if (currentNode == &nullTree)
                        break;
                }

                if (currentNode != &nullTree)
                    csvRow.push_back(currentNode->get_value<std::string>());
                else
                    csvRow.push_back("N/A");
            }

            // Print the CSV row
            std::cout << boost::algorithm::join(csvRow, ",") << "\n";
        }
    }


    static void printJsonToXml(const std::string& jsonStr, const std::vector<std::string>& keys)
    {
        std::istringstream is(jsonStr);
        pt::ptree jsonResponse;
        pt::read_json(is, jsonResponse);

        const auto& results = jsonResponse.get_child("result");

        pt::ptree xmlOutput;

        int i = 0;
        pt::ptree nullTree;
        for (const auto& kv : results)
        {
            const auto& obj = kv.second;

            pt::ptree xmlRow;

            for (const auto& key : keys) 
            {
                std::vector<std::string> keyParts;
                boost::split(keyParts, key, boost::is_any_of("."));

                const pt::ptree* currentNode = &obj;
                for (const auto& part : keyParts)
                {
                    currentNode = &currentNode->get_child(part, nullTree);
                    if (currentNode == &nullTree)
                        break;
                }
                if (currentNode != &nullTree)
                    xmlRow.put(key, currentNode->get_value<std::string>());
                else
                    xmlRow.put(key, "N/A");            
            }

            xmlOutput.add_child("item" + std::to_string(i++), xmlRow);
        }

        pt::write_xml(std::cout, xmlOutput);
    }
};

 #endif