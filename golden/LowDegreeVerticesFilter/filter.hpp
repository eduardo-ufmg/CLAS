#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>
#include <map>
#include <cmath>

void buildGraph(std::vector< std::vector<int> > &adj, const std::vector< std::pair<int,int> > &edges, const size_t numVertices);
void computeQ(std::vector<double> &q, const std::vector< std::vector<int> > &adj, const std::vector<int> &targets, const size_t numVertices);
void computeClassStats(std::map<int, std::pair<double, int>> &classStats, const std::vector<int> &targets, const std::vector<double> &q, const size_t numVertices);
void computeThresholds(std::map<int, double> &threshold, const std::map<int, std::pair<double, int>> &classStats);
void filterVertices(std::vector<bool> &keep, const std::vector<double> &q, const std::map<int, double> &threshold, const std::vector<int> &targets, const size_t numVertices);
int reindexVertices(std::vector<int> &newIndex, const std::vector<bool> &keep, const size_t numVertices);

#endif // FILTER_HPP
