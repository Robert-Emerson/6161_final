/* BoruvkaNode.h: Boruvka node class definition */
#ifndef BORUVKA_NODE_H
#define BORUVKA_NODE_H

#include <vector>

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/pending/disjoint_sets.hpp"

//typedef that allows easy use of the boost edge_weight property
//Edge weights are integer values that express the cost of traversing an edge between two nodes
typedef boost::property< boost::edge_weight_t, int> edge_weight;

//typedef that allows easy use of the boost adjaceny_list as a Graph
//Uses boost vectors to store vertices and edges. This is an undirected graph.
typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, edge_weight > Graph;

typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::vertices_size_type vertices_size_type;

typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
typedef boost::property_map< Graph, boost::edge_weight_t>::type EdgeWeightMap;
typedef std::pair<edge_descriptor, int> w_edge;

class BoruvkaNode
{
  public:
    BoruvkaNode();
    BoruvkaNode(vertex_descriptor);
    ~BoruvkaNode();
    
    void addChild( BoruvkaNode* child);
    void setParent( BoruvkaNode* parent, int weight);
    
    int getType() const;
    std::vector<BoruvkaNode*>& getChildren();
    BoruvkaNode* getParent();
    
  private:
    std::vector<BoruvkaNode*> empty;
    std::vector<BoruvkaNode*> children;
    BoruvkaNode* parent;
    int weightToParent;
    int constructorType;
};
#endif
