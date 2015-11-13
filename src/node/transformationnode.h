#ifndef TRANSFORMATIONNODE_H
#define TRANSFORMATIONNODE_H

#include "node.h"
#include "transformmatrix.h"

class TransformationNode : public Node
{
public:
	TransformationNode();
	void accept(NodeVisitor&);
	TransformMatrix* getMatrix() const;
	void setMatrix(TransformMatrix*);
private:
	TransformMatrix* matrix;
};

#endif // TRANSFORMATIONNODE_H
