/* BoruvkaNode.h: Boruvka node class definition */
#ifndef BORUVKA_NODE_H
#define BORUVKA_NODE_H

#include <list>

class BoruvkaNode
{
    std::list<BoruvkaNode*> children;
    BoruvkaNode* parent;
    int weightToParent;

    BinaryNode( const int theWeight, BinaryNode *lt, BinaryNode *rt )
            : weightToParent( theWeight ), parent( NULL ) { }
    friend class BoruvkaTree;
};

#endif
