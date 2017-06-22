#ifndef __PE_ROW_H__
#define __PE_ROW_H__

#include "common.h"
#include "pe.h"
#include "broadcast.h"
#include "tag_filter.h"

class pe_row : public sc_module
{
public:
    tlm_target_socket<> isocket;
    tlm_target_socket<> fsocket;
    sc_vector< tlm_target_socket<> > psocket;
    sc_vector< tlm_initiator_socket<> > opsocket;

    sc_vector< pe > elements;

protected:
    unsigned int m_size;
    broadcast ibroadcast;
    broadcast fbroadcast;
    sc_vector< tag_filter > itags;
    sc_vector< tag_filter > ftags;
   
public:
    pe_row (const sc_module_name& nm, unsigned int size);

protected:
};

#endif /* __PE_ROW_H__ */
