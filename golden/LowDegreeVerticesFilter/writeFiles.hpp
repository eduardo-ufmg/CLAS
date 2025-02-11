#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <vector>
#include <string>

int writeFilteredVertices(const std::vector< std::vector<double> > &vertices, const std::vector<bool> &keep, const std::string &filtered_features_file_name_with_path);
int writeFilteredTargets(const std::vector<int> &targets, const std::vector<bool> &keep, const std::string &filtered_targets_file_name_with_path);
int writeFilteredEdges(const std::vector< std::pair<int,int> > &edges, const std::vector<bool> &keep, const std::vector<int> &newIndex, const std::string &filtered_edges_file_name_with_path);

#endif // WRITEFILES_H
