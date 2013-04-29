/* Code directly adapted from T. Hagerup's MST Verification code written in D */
#include "verification.hpp"

MSTVerifier::MSTVerifier( BoruvkaTree treeIn, std::vector<int> weight,std::vector<int> upper,std::vector<int> lower)
{
  tree = BoruvkaTree(treeIn);
  this->height = 0;
  this->upper = upper;
  this->lower = lower;
  this->weight = weight;
  
  this->child = tree.getChildren();
  this->sibling = tree.getSiblings();
}

std::vector<int> MSTVerifier::treePathMaxima()
{
  L = std::vector<int>(tree.getChildren().size(), -1);
  Lnext = std::vector<int>(upper.size(), -1);
  
  for (int i=0;i<m;i++) { // distribute queries to lower nodes
    Lnext[i]=L[lower[i]];
    L[lower[i]]=i;
  }
  
  return tree_path_maxima( tree.getRootInt(), child, sibling, weight, upper, lower);
  
}

std::vector<int> MSTVerifier::tree_path_maxima(int root,std::vector<int> child,std::vector<int> sibling, std::vector<int> weight,std::vector<int> upper,std::vector<int> lower) {
  
  init(root,0);
  P = std::vector<int>(0,height+1);
  median=median_table(height);
  visit(root,0);
  return answer;
} // end tree_path_maxima

void MSTVerifier::init(int u,int d) { // d = depth of u
    depth[u]=d;
    if (d>height) height=d; // height of T = maximum depth
    for (int i=L[u];i>=0;i=Lnext[i])
	D[u]|=1<<depth[upper[i]];
    for (int v=child[u];v>=0;v=sibling[v]) {
      init(v,d+1);
      D[u]|=D[v]&~(1<<d);
    }
  }
  
  
// Stores the subsets of size k of {0,...,n-1} in T,
// starting in position p, and returns p plus their number.
int MSTVerifier::subsets(int n,int k,int p, std::vector<int> T) {
  if (n<k) return p;
  if (k==0) { T[p]=0; return p+1; }
  int q=subsets(n-1,k-1,p,T);
  for (int i=p;i<q;i++) T[i]|=1<<(n-1);
  return subsets(n-1,k,q,T);
  }//end subsets
  
// Returns a table of size 2^(h+1) whose entry in position i,
// i=0,...,2^(h-1)-1, is the median of the set represented by

std::vector<int> MSTVerifier::median_table(int h) {
   std::vector<int> T= std::vector<int>((1<<h)+1, -1),median= std::vector<int>(1<<h+1, -1);
    for (int s=0;s<=h;s++) {
      for (int k=0;k<=s;k++) {
	int p=subsets(h-s,k,0, T);
	int q=subsets(s,k,p, T);
	q=subsets(s,k+1,q, T);
	for (int i=0;i<p;i++) {
	  int b=(1<<s+1)*T[i]+(1<<s); // fixed high bits
	  for (int j=p;j<q;j++)
	    median[b+T[j]]=s; // variable low bits
	}
      }
    }
  return median;
} // end median_table
  
// Returns A "downarrow" B
int MSTVerifier::down(int A,int B) {
  return B&(~(A|B)^(A+(A|~B)));
}

void MSTVerifier::visit(int v,int S) { // S = S of parent

  P[depth[v]]=v; // push current node on stack
  int k=binary_search(weight[v],down(D[v],S));
  S=down(D[v],S&(1<<(k+1)-1)|(1<<depth[v]));
  
  for (int i=L[v];i>=0;i=Lnext[i])
    answer[i]=P[median[down(1<<depth[upper[i]],S)]];
  
  for (int z=child[v];z>=0;z=sibling[z]) visit(z,S);
} // end visit

int MSTVerifier::binary_search(int w,int S) {
  // Returns max({j in S | weight[P[j]]>w} union {0})
  if (S==0) return 0;
  int j=median[S];
  while (S!=1<<j) 
  { // while |S|>1
    S&=(weight[P[j]]>w)?~((1<<j)-1):(1<<j)-1;
    j=median[S];
  }
  return (weight[P[j]]>w)?j:0;
}
    
