//Verification hpp
//Constructs a verifier, then runs it on a Boruvka Tree.

#ifndef VERIFICATION_CPP
#define VERIFICATION_CPP

#include <vector>
#include "boruvka_tree/BoruvkaTree.hpp"

class MSTVerifier{

  public:
    //upper and lower hold the source and destination of edges in original graph
    MSTVerifier( BoruvkaTree treeIn, std::vector<int> weight, std::vector<int> upper, std::vector<int> lower);

    std::vector<int> treePathMaxima(); 

  private:
    BoruvkaTree tree;
    int height;
    int n;
    int m;
    std::vector<int> depth, D, L, Lnext, answer, median, P, weight, upper, lower, child, sibling;

    void init( int u, int d);
    int subsets(int n,int k,int p, std::vector<int> T);
    std::vector<int> median_table( int h);
    int down( int A, int B);
    void visit( int v, int S);
    int binary_search( int w, int S);
    std::vector<int> tree_path_maxima(int root,std::vector<int> child,std::vector<int> sibling, std::vector<int> weight,std::vector<int> upper,std::vector<int> lower);
};
#endif
