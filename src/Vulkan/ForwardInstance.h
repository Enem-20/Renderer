#ifndef C_FORWARD_INSTANCE_H
#define C_FORWARD_INSTANCE_H

class ForwardInstanceImpl;

class ForwardInstance {
public:
    ForwardInstance();
    ~ForwardInstance();
private:
    ForwardInstanceImpl* impl;
};

#endif