/* BoruvkaNode.h: Boruvka node class definition */
#ifndef BORUVKA_NODE_H
#define BORUVKA_NODE_H

#include <list>

class BoruvkaNode
{
  public:
    BoruvkaNode();
    BoruvkaNode(vertex_descriptor);
    ~BoruvkaNode();
    
    void addChild( BoruvkaNode* child);
    void setParent( BoruvkaNode* parent, int weight);
    
    int getType();
    
  private:
    std::list<BoruvkaNode*> children;
    BoruvkaNode* parent;
    int weightToParent;
    int constructorType;
};

#endif
