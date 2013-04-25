/*
 * Robert Emerson
 */

#include "BoruvkaTree.hpp"


/**
* Construct the tree.
*/
BoruvkaTree::BoruvkaTree( int numLeaves )
{
  this->numNodes = numLeaves;
}


/**
* Destructor for the tree.
*/
BoruvkaTree::~BoruvkaTree( )
{
    this->makeEmpty();
}

/**
 * Create all the leaves of the tree
 */
void BoruvkaTree::create( std::vector<vertex_descriptor> vertices)
{
  std::vector<vertex_descriptor>::iterator first = vertices.begin();
  for( ; first != vertices.end(); ++first)
  {
    if (vertexToNode[*first]->getType() == 0)
    {
      BoruvkaNode* newNode = new BoruvkaNode(*first);
      newNode->setParent( root, -1);
      vertexToNode[*first] = newNode;
      nodeToVertex[newNode] = *first;
    }
  }
}

/**
 * Sets the parent for the selected vertex descriptor
 */
void BoruvkaTree::setParent( vertex_descriptor child, vertex_descriptor parent, int weight)
{
  if (vertexToNode[parent]->getType() == 0)
  {
    BoruvkaNode* newNode = new BoruvkaNode(parent);
    newNode->setParent( root, -1);
    vertexToNode[parent] = newNode;
    nodeToVertex[newNode] = parent;
  }
  setParent(vertexToNode[child], vertexToNode[parent], weight);
}

void BoruvkaTree::setParent( BoruvkaNode* child, BoruvkaNode* parent, int weight)
{
  child->setParent(parent, weight);
  parent->addChild(child);
}

/**
* Make the tree logically empty.
*/
void BoruvkaTree::makeEmpty( )
{
    // call the private makeEmpty() routine
    makeEmpty( root );
}

BoruvkaNode* BoruvkaTree::getRoot() const
{
  return this->root;
}

//need to implement getChildren and getSiblings still

/**
* Internal method to make subtree empty.
*/
void BoruvkaTree::makeEmpty( BoruvkaNode* t )
{
    if ( t != NULL )
    {
        //for all children of t, call MakeEmpty
        //@TODO
        delete t;
    }
    t = NULL;
}
