#include "PrepareRequestFromOpt.hpp"

void mergePropertyTrees(pt::ptree& dest, const pt::ptree& src)
{
    for (const auto& pair : src)
        dest.add_child(pair.first, pair.second);
}

pt::ptree prepareJsonBody(const ReqConfig& options)
{
    pt::ptree params;

	
	pt::ptree parameters;
	for (const auto& parameter : options.parameters)
	{
        auto delimiterPos = parameter.find(':');
        auto path = parameter.substr(0, delimiterPos);
        auto value = parameter.substr(delimiterPos + 1);
		parameters.put(path, value);
	}

	if (!options.parameters.empty())
		mergePropertyTrees(params, parameters);


	pt::ptree output;
    for (const auto& key : options.outputs)
	{
        pt::ptree key_node;
        key_node.put("", key);
        output.push_back(std::make_pair("", key_node));
    }

    
	if (!options.outputs.empty())
    	params.add_child("output", output);
	
    pt::ptree filterTree;
    for (const auto& filter : options.filters)
	{
        auto delimiterPos = filter.find(':');
        auto path = filter.substr(0, delimiterPos);
        auto value = filter.substr(delimiterPos + 1);
        std::istringstream pathStream(path);
        std::string pathSegment;
        pt::ptree* currentNode = &filterTree;
        while (std::getline(pathStream, pathSegment, '.'))
            currentNode = &currentNode->put_child(pathSegment, pt::ptree());
		
        currentNode->put_value(value);
    }

    if (!options.filters.empty())
        params.put_child("filter", filterTree);


	return params;
}