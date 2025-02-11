#ifndef READFILES_HPP
#define READFILES_HPP

#include <vector>
#include <string>

int readVertices(std::vector< std::vector<double> > &vertices, const std::string &features_file_name_with_path);
int readTargets(std::vector<int> &targets, const std::string &targets_file_name_with_path);
int readEdges(std::vector< std::pair<int,int> > &edges, const std::string &edges_file_name_with_path);

#endif // READFILES_HPP
