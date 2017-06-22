#ifndef __TOP_H__
#define __TOP_H__

#include "common.h"
#include "cpu_model.h"
#include "ram.h"
#include "eyeriss.h"

class top : public sc_module
{
protected:
    unsigned int m_hsize = sknobs_get_value("eyeriss.hsize", 0);
    unsigned int m_vsize = sknobs_get_value("eyeriss.vsize", 0);
    cpu_model   cpu;
    ram         dram;
    eyeriss     eyeriss_0;

public:
    top(const sc_module_name& nm);
    void end_of_simulation ();
};

#endif /* __TOP_H__ */
