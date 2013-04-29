/*
 * Robert Emerson
 */

#include "BoruvkaNode.hpp"
#include <iostream>

/**
 * Default construct the node
 */
BoruvkaNode::BoruvkaNode()
{
  this->children = std::vector<BoruvkaNode*>();
  this->children.clear();
  this->empty = std::vector<BoruvkaNode*>();
  this->parent = NULL;
  this->constructorType = 0; //Signifies default constructor
  this->weightToParent = -1;
}

BoruvkaNode::~BoruvkaNode()
{
  delete this->parent;
}

/**
 * Vertex descriptor constructor
 */
BoruvkaNode::BoruvkaNode( vertex_descriptor vertex)
{
  this->children = std::vector<BoruvkaNode*>();
  this->children.clear();
  this->empty = std::vector<BoruvkaNode*>();
  this->parent = NULL;
  this->constructorType = 1; //Signifies non-default constructor
  this->weightToParent = -1;
}

void BoruvkaNode::addChild( BoruvkaNode* child)
{
  children.push_back(child);
}

void BoruvkaNode::setParent (BoruvkaNode* parent, int weight)
{
  this->parent = parent;
  this->weightToParent = weight;
}

int BoruvkaNode::getType() const
{
  return this->constructorType;
}

BoruvkaNode* BoruvkaNode::getParent()
{
  return this->parent;
}

std::vector<BoruvkaNode*>& BoruvkaNode::getChildren()
{
  if ( &(children.front()) != NULL)
    return children;
  return empty;
}