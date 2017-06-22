
#ifndef __CPU_MODEL_H__
#define __CPU_MODEL_H__

#include "common.h"
#include "protocols.h"

class cpu_model : public sc_module
{
public:
    simple_initiator_socket< cpu_model > socket;
    
protected:
    
public:
    cpu_model (const sc_module_name& nm);
    
protected:
    SC_HAS_PROCESS(cpu_model);
    void operation ();
};

#endif /* __CPU_MODEL_H__ */
