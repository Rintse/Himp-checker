#ifndef node_h
#define node_h 

class Node {
    public:
        Node();
        ~Node();
};

class BlockNode {
    
};

class IDNode {
    IDNode(char _op);
};

class AOpNode : public Node {
    public:
        AOpNode(char _op, Node* _l, Node* _r);
    private:
        char op;
        Node* l;
        Node* r;
};

#endif
