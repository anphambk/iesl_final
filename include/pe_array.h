#ifndef __PE_ARRAY_H__
#define __PE_ARRAY_H__

#include "common.h"
#include "pe_row.h"
#include "tag_filter.h"
#include "broadcast.h"

class pe_array : public sc_module
{
public:
    tlm_target_socket<> isocket;
    tlm_target_socket<> fsocket;
    sc_vector< tlm_target_socket<> > psocket;
    sc_vector< tlm_initiator_socket<> > opsocket;
   
    sc_vector< pe_row > rows;

protected:
    unsigned int m_hsize;
    unsigned int m_vsize;
    broadcast ibroadcast;
    broadcast fbroadcast;
    
public:
    pe_array (const sc_module_name& nm, unsigned int hsize, unsigned int vsize);

protected:
    pe_row* create_row(const sc_module_name& nm, size_t);
};

#endif /* __PE_ARRAY_H__ */
