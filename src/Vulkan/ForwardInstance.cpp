#include "ForwardInstance.h"
#include "Instance.h"
#include "RenderPass.h"

class ForwardInstanceImpl : public Instance {
public:
    ForwardInstanceImpl() 
        : Instance("ForwardInstance", {"VK_LAYER_KHRONOS_validation"})
    {
        addRenderPass("Forward", new RenderPass("Forward", this));
    }

    ~ForwardInstanceImpl() {
        
    }
};

ForwardInstance::ForwardInstance() 
    : impl(new ForwardInstanceImpl())
{

}

ForwardInstance::~ForwardInstance() {
    delete impl;
}