#ifndef PREPARE_REQUEST_FROM_OPT_HPP
 #define PREPARE_REQUEST_FROM_OPT_HPP

#include "GetProgramOptions.hpp"
#include "ZabbixApiClient.hpp"
#include <boost/algorithm/string.hpp>

void mergePropertyTrees(pt::ptree& dest, const pt::ptree& src);

pt::ptree prepareJsonBody(const ReqConfig& options);


#endif