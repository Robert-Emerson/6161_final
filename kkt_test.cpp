/**
 * @author Robert Emerson
 * @date March 6, 2013
 * @brief KKT randomized MST algorithm.
 */

#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>
#include <ctime>

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/kruskal_min_spanning_tree.hpp"
#include "boost/pending/disjoint_sets.hpp"

#include "verification.hpp"
#include "boruvka_tree/BoruvkaNode.hpp"
#include "boruvka_tree/BoruvkaTree.hpp"

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
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_iterator;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
typedef boost::property_map< Graph, boost::edge_weight_t>::type EdgeWeightMap;
typedef std::pair<edge_descriptor, int> w_edge;

typedef vertices_size_type* Rank;
typedef vertex_descriptor* Parent;

const int numNodes = 10000000; //must be >= 10. is the max number of nodes
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
Graph boruvkaCut( Graph& graph, int createTree, BoruvkaTree& boruvkaTree);

w_edge findMinWeightEdge( w_edge edge1, w_edge edge2);

int main( int argc, char* argv[])
{
  clock_t begin, end;
  double time_spent;
  std::srand (time(NULL));
  
  Graph graph( numNodes);
  Graph graph2( numNodes);

  begin = clock();
  createGraph( graph);
  graph2 = graph;
  
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  
  std::cout << "Init took: " << time_spent << " seconds." << std::endl;
  

  begin = clock();
  std::vector < edge_descriptor > spanning_tree;
  boost::kruskal_minimum_spanning_tree(graph2, std::back_inserter(spanning_tree));
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  
  std::cout << "Kruskal MST took: " << time_spent << " seconds." << std::endl;
  
  kktMST(graph);
  
  graph.clear();
  return 0;
}

Graph kktMST( Graph& graph)
{
  std::cout << boost::num_edges( graph) << std::endl;
  if ( boost::num_edges( graph) > 1)
  {
    Graph graphTemp( boost::num_vertices( graph));
    
    BoruvkaTree boruvkaTree = BoruvkaTree( boost::num_vertices( graph), supervertex_map);
    edge_iterator edgeBegin, edgeEnd, nextEdge;
    
    graphTemp = boruvkaCut( graph, 0, boruvkaTree);
    
    Graph graph2( boost::num_vertices( graphTemp));
    graph2 = boruvkaCut( graphTemp, 1, boruvkaTree );
    
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph2);
    
    for (nextEdge = edgeBegin; edgeBegin != edgeEnd; edgeBegin = nextEdge)
    {
      if( std::rand() % 10 / 10 > .5)
      {
	++nextEdge;
	boost::remove_edge( *edgeBegin , graph2);
      }
    }
  
    Graph graph3( boost::num_vertices(graph2));
    
    graph3 = kktMST( graph2);
    EdgeWeightMap weightMap = boost::get(boost::edge_weight_t(), graph3);
    
    std::vector<int> weight, upper, lower;
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph3);
    for (nextEdge = edgeBegin; edgeBegin != edgeEnd; ++edgeBegin)
    {
      int edgeWeight = boost::get(weightMap, *edgeBegin);
      
      weight.push_back( edgeWeight );
      upper.push_back(source(*edgeBegin, graph3));
      lower.push_back(target(*edgeBegin, graph3));
    }
    
    MSTVerifier verify = MSTVerifier(boruvkaTree, weight, upper, lower);  
    
    std::vector<int> fHeavy = verify.treePathMaxima();
    
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph3);
    int fHeavyCount = 0;
    for (nextEdge = edgeBegin; edgeBegin != edgeEnd; edgeBegin = nextEdge)
    {
      ++nextEdge;
      if ( fHeavy[fHeavyCount] < boost::get(weightMap, *edgeBegin) )
	boost::remove_edge( *edgeBegin , graph3);
    }
    
    Graph graph4 ( boost::num_vertices( graph3));
    graph4 = kktMST(graph3);
    
    graph3.clear();
    graph2.clear();
    
    return graph4;
  }
  else
  {
    return graph;
  }
}

/*
void createGraph( Graph& graph)
{
  
  for( int i = 0; i < numNodes; i++)
  {
    for( int j = 0; j < numNodes; j++)
    {
      if( std::rand() >= RAND_MAX/(numNodes/10) && i != j) //at most 10 edges per node
      {
	edge_weight ewp = std::rand()%50; //assigns each edge a weight between 0 and 50
	add_edge( i, j, ewp, graph);
	dset.make_set( vertex(i, graph));
	supervertex_map[i] = vertex(i,graph);
      }
    }
  }
  
  std::cout << "Create graph " << boost::num_edges( graph) << std::endl;
}
*/

void createGraph( Graph& graph)
{
  vertex_iterator vertexBegin, vertexEnd;
  vertex_iterator vertexBegin1, vertexEnd1;
  
  boost::tie(vertexBegin, vertexEnd) = boost::vertices(graph);
  boost::tie(vertexBegin1, vertexEnd1) = boost::vertices(graph);
  
  for( ; vertexBegin != vertexEnd; ++vertexBegin)
  {
    for( ; vertexBegin1 != vertexEnd1; ++vertexBegin1)
    {
      if( std::rand() >= RAND_MAX/(numNodes/10) && *vertexBegin != *vertexBegin1)
      {
	edge_weight ewp = std::rand()%50; //assigns each edge a weight between 0 and 50
	add_edge( *vertexBegin, *vertexBegin1, ewp, graph);
	dset.make_set( vertex(*vertexBegin, graph));
	supervertex_map[*vertexBegin] = vertex(*vertexBegin,graph);
      }
    }
  }
  
  std::cout << "Create graph " << boost::num_edges( graph) << std::endl;
}

Graph boruvkaCut( Graph& graph, int createTree, BoruvkaTree& boruvkaTree)
{ 
  
  std::cout << "Boruvka Cut " <<boost::num_edges( graph) << std::endl;
  if (createTree == 0) //first cut, don't need to create tree
  {
    EdgeWeightMap weight = boost::get(boost::edge_weight_t(), graph);
    vertex_iterator vertexBegin, vertexEnd;
    edge_iterator edgeBegin, edgeEnd;
    edge_descriptor minWeightEdge;
    Graph graph2(boost::num_vertices(graph));
    
    const int infinity = (std::numeric_limits<int>::max)();
    std::vector<w_edge> candidate_edges( supervertices.size());
    
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph);
    
    for (; edgeBegin != edgeEnd; ++edgeBegin)
    {
      std::pair<edge_descriptor, int> insertEdge = w_edge( *edgeBegin, infinity);
      candidate_edges.push_back( insertEdge);
      //std::cout << insertEdge.first << " " << insertEdge.second << std::endl;
    }

    
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
	  vertex_descriptor findU = dset.find_set(u);
	  if ( findU == u)
	      victim = v;
	  if ( &supervertex_map[victim] == NULL)
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
	

	vertex_iterator vertexBeginGraph2, vertexEndGraph2;  
    for( boost::tie(vertexBeginGraph2, vertexEndGraph2) = boost::vertices(graph2); vertexBeginGraph2 != vertexEndGraph2; ++vertexBeginGraph2)
	{
	  out_edge_iterator outEdgeBegin, outEdgeEnd;
	  vertex_descriptor v = *vertexBeginGraph2;
	  boost::tie(outEdgeBegin, outEdgeEnd) = out_edges(v, graph);
	  for ( ; outEdgeBegin != outEdgeEnd; ++outEdgeBegin)
	  {
	    vertex_descriptor outVertex = dset.find_set( target( *outEdgeBegin, graph));
	    if ( outVertex != v)
	    {
		boost::add_edge(v, outVertex, boost::get(weight, *outEdgeBegin), graph2);
	    }
	  }
	}
    supervertices.erase( std::remove( supervertices.begin(), supervertices.end(),
				      boost::graph_traits<Graph>::null_vertex()),
			  supervertices.end());
    
  std::cout << "Boruvka Cut V Graph2: " << boost::num_vertices( graph2) << std::endl;  
  std::cout << "Boruvka Cut E Graph2: " << boost::num_edges( graph2) << std::endl;
    return graph2;
  }
  else //Create the boruvka tree in this step
  {
    vertex_iterator vertexBegin, vertexEnd;
    std::vector<vertex_descriptor> vertexDescriptors;
    std::map<vertex_descriptor, std::list<vertex_descriptor> > parentMap;
    
    EdgeWeightMap weight = boost::get(boost::edge_weight_t(), graph);
    edge_iterator edgeBegin, edgeEnd;
    edge_descriptor minWeightEdge;
    Graph graph2;
    
    boost::tie( vertexBegin, vertexEnd) = boost::vertices( graph);
    
    for (; vertexBegin != vertexEnd; ++vertexBegin)
    {
      vertexDescriptors.push_back(*vertexBegin);
    }
    boruvkaTree.create(vertexDescriptors);
    
    const int infinity = (std::numeric_limits<int>::max)();
    std::vector<w_edge> candidate_edges( supervertices.size());
    
    boost::tie( edgeBegin, edgeEnd) = boost::edges( graph);
    
    for (; edgeBegin != edgeEnd; ++edgeBegin)
    {
      std::pair<edge_descriptor, int> insertEdge = w_edge( *edgeBegin, infinity);
      candidate_edges.push_back( insertEdge);
    }
    
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
	  
	  boruvkaTree.setParent(u,v, boost::get(weight, e));

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
	

	vertex_iterator vertexBeginGraph2, vertexEndGraph2;  
    for( boost::tie(vertexBeginGraph2, vertexEndGraph2) = boost::vertices(graph2); vertexBeginGraph2 != vertexEndGraph2; ++vertexBeginGraph2)
	{
	  out_edge_iterator outEdgeBegin, outEdgeEnd;
	  vertex_descriptor v = *vertexBeginGraph2;
	  boost::tie(outEdgeBegin, outEdgeEnd) = out_edges(v, graph);
	  for ( ; outEdgeBegin != outEdgeEnd; ++outEdgeBegin)
	  {
	    vertex_descriptor outVertex = dset.find_set( target( *outEdgeBegin, graph));
	    if ( outVertex != v)
	    {
		boost::add_edge(v, outVertex, boost::get(weight, *outEdgeBegin), graph2);
	    }
	  }
	}
    supervertices.erase( std::remove( supervertices.begin(), supervertices.end(),
				      boost::graph_traits<Graph>::null_vertex()),
			  supervertices.end());
     std::cout << "Boruvka Cut Tree Graph2: " << boost::num_edges( graph2) << std::endl;
    return graph2;
  }
  
}

w_edge findMinWeightEdge( w_edge edge1, w_edge edge2)
{
  if ( edge1.second < edge2.second) return edge1;
  else return edge2;
}

