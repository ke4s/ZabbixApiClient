#include "PTreePrinter.hpp"


void PTreePrinter::printStringToJson(const std::string& jsonStr)
{
    std::istringstream is(jsonStr);
    pt::ptree jsonResponse;
    pt::read_json(is, jsonResponse);
    
    pt::write_json(std::cout, jsonResponse.get_child("result"));
}

void PTreePrinter::printJsonToCsv(const std::string& jsonStr, const std::vector<std::string>& keys)
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
            std::string temp;
            if (currentNode != &nullTree)
                csvRow.push_back(((temp = currentNode->get_value<std::string>("N/A")) == "" ? "N/A" : temp)); // value "" gelirse yerine "N/A" yazar
            else
                csvRow.push_back("N/A");
        }

        std::cout << boost::algorithm::join(csvRow, ",") << "\n";
    }
}