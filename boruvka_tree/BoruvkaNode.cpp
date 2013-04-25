/*
 * Robert Emerson
 */

#include "BoruvkaNode.hpp"

/**
 * Default construct the node
 */
BoruvkaNode::BoruvkaNode()
{
  this.parent = NULL;
  this.constructorType = 0; //Signifies default constructor
  this.weightToParent = -1;
}

/**
 * Vertex descriptor constructor
 */
BoruvkaNode::BoruvkaNode( vertex_descriptor vertex)
{
  this.parent = NULL;
  this.constructorType = 1; //Signifies non-default constructor
  this.weightToParent = -1;
}

void BoruvkaNode::addChild( BoruvkaNode* child)
{
  childern.insert(child);
}

void BoruvkaNode::setParent (BoruvkaNode* parent, int weight)
{
  this.parent = parent;
  this.weightToParent = weight;
}

int getType()
{
  return constructorType;
}