#ifndef BORUVKA_TREE_H
#define BORUVKA_TREE_H


#include "BoruvkaNode.hpp"
#include <iostream>       // For NULL

#include <cstdlib>
#include <iostream>
#include <tuple>
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

class BoruvkaTree
{
  public:
      BoruvkaTree(int numLeaves);
      BoruvkaTree( const BoruvkaTree & rhs );
      ~BoruvkaTree( );

      void create( std::vector<vertex_descriptor> vertices);
      void setParent( vertex_descriptor child, vertex_descriptor parent);
      void setParent( BoruvkaNode* child, BoruvkaNode* parent, int weight);
      void makeEmpty();
      
      BoruvkaNode* getRoot() const;
      std::vector<BoruvkaNode*> getChildren() const;
      std::vector<BoruvkaNode*> getSiblings() const;

  private:
      BoruvkaNode* root;
      int numNodes;
      std::map< vertex_descriptor, BoruvkaNode*> vertexToNode;
      std::map< BoruvkaNode*, vertex_descriptor> nodeToVertex;
      void makeEmpty(BoruvkaNode* node);

      const string & elementAt( BoruvkaNode* t ) const;
    
};

#endif
