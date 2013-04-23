/**
 * @author Robert Emerson
 * @date March 6, 2013
 * @brief KKT randomized MST algorithm.
 */

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

typedef vertices_size_type* Rank;
typedef vertex_descriptor* Parent;

const int numNodes = 10000;
std::vector<vertices_size_type> rank(numNodes);
std::vector<vertex_descriptor> parent(numNodes);
boost::disjoint_sets< Rank, Parent> dset( &rank[0], &parent[0]);
std::map<int, vertex_descriptor> supervertex_map;
std::vector<vertex_descriptor> supervertices;

/**
 * @var boost::adjacency_list - graph - Adjacency List representation of a graph
 * @var int - numNodes - Number of nodes our graph contains
 * @return void Updates the graph instead of returning anything
 * Fills the graph with nodes and edges. Each vertex has a 1/1000 chance of beinga
 * connected to another vertex, meaning each vertex will have 10 outgoing edges
 * on average.
 */
void createGraph( Graph& graph);

Graph kktMST( Graph& graph);

/**
 * @var boost::adjacency_list - graph - Adjacency List representation of a graph
 * @return graph Returns a condensed version of the graph
 * Condenses the graph using the Boruvka algorithm
 */
Graph boruvkaCut( Graph graph);

w_edge findMinWeightEdge( w_edge edge1, w_edge edge2);

int main( int argc, char* argv[])
{
  //Constructing our graph with 10000 nodes
  Graph graph( numNodes);

  createGraph( graph);
  
  kktMST(graph);
  
  graph.clear();
  return 0;
}

Graph kktMST( Graph& graph)
{
  if ( num_edges( graph) != 1)
  {
    Graph graph2( boost::num_vertices(graph));
    edge_iterator edgeBegin, edgeEnd, nextEdge;
    
    graph2 = boruvkaCut( boruvkaCut( graph) );
    
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph2);
    
    if(boost::num_edges(graph2) / boost::num_vertices (graph2) >= 6 )
    {
      for (nextEdge = edgeBegin; edgeBegin != edgeEnd; edgeBegin = nextEdge)
      {
	if( std::rand() % 10 / 10 > .5)
	{
	  ++nextEdge;
	  boost::remove_edge( *edgeBegin , graph2);
	}
      }
    }
    Graph graph3( boost::num_vertices(graph2));
    
    graph3 = kktMST( graph2);
    //verification code will go here, probably as a separate method
    //run verification on graph using graph3 converted to tree
    
    graph3.clear();
    graph2.clear();
  }
 
  return graph;
}

void createGraph( Graph& graph)
{
  
  for( int i = 0; i < numNodes; i++)
  {
    for( int j = 0; j < numNodes; j++)
    {
      if( std::rand() >= RAND_MAX/(numNodes/10) && i != j)
      {
	edge_weight ewp = std::rand()%50; //assigns each edge a weight between 0 and 50
	add_edge( i, j, ewp, graph);
	dset.make_set( vertex(i, graph));
	supervertex_map[i] = vertex(i,graph);
      }
    }
  }
}

Graph boruvkaCut( Graph graph)
{  
  EdgeWeightMap weight = boost::get(boost::edge_weight_t(), graph);
  vertex_iterator vertexBegin, vertexEnd;
  edge_iterator edgeBegin, edgeEnd;
  edge_descriptor minWeightEdge;
  Graph graph2;
  
  const int infinity = (std::numeric_limits<int>::max)();
  std::vector<w_edge> candidate_edges( supervertices.size(), w_edge( edge_descriptor(), infinity));
  
  boost::tie( edgeBegin, edgeEnd) = boost::edges( graph);
  
  for (; edgeBegin != edgeEnd; ++edgeBegin)
  {
    int w = boost::get(weight, *edgeBegin);
    int u = supervertex_map[dset.find_set(source(*edgeBegin, graph))];
    int v = supervertex_map[dset.find_set(target(*edgeBegin, graph))];

    if (u != v)
    {
      candidate_edges[u] = findMinWeightEdge( candidate_edges[u], w_edge( *edgeBegin, w));
      candidate_edges[v] = findMinWeightEdge( candidate_edges[v], w_edge( *edgeBegin, w));
    }
  }
  
  for (int i = 0; i < candidate_edges.size(); ++i)
  {
    if (candidate_edges[i].second != infinity)
    {
      edge_descriptor e = candidate_edges[i].first;
      vertex_descriptor u = dset.find_set( source(e, graph));
      vertex_descriptor v = dset.find_set( target(e, graph));
      if (u != v)
      {
	// Link the two supervertices
	dset.link(u, v);

	// Whichever vertex was reparented will be removed from the
	// list of supervertices.
	vertex_descriptor victim = u;
	if (dset.find_set(u) == u)
	    victim = v;
	supervertices[supervertex_map[victim]] = boost::graph_traits<Graph>::null_vertex();
      }
    }
  }
  //given a vector of vertices from supervertices, form graph.
  
  for( std::vector<vertex_descriptor>::iterator vertexBegin = supervertices.begin();
       vertexBegin != supervertices.end(); ++vertexBegin)
      {
	vertex_descriptor v = add_vertex(graph2);
	v = *vertexBegin;
      }
  supervertices.erase( std::remove( supervertices.begin(), supervertices.end(),
                                    boost::graph_traits<Graph>::null_vertex()),
                        supervertices.end());
  return graph2;
  
}

w_edge findMinWeightEdge( w_edge edge1, w_edge edge2)
{
  if ( edge1.second < edge2.second) return edge1;
  else return edge2;
}

