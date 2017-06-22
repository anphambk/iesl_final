#ifndef __TAG_FILTER_H__
#define __TAG_FILTER_H__

#include "common.h"

class tag_filter : public sc_module
{
public:
    //simple_target_socket< tag_filter > csocket;
    simple_target_socket< tag_filter > isocket;
    simple_initiator_socket< tag_filter > osocket;

protected:
    unsigned int m_address;
    unsigned int m_tag;

public:
    tag_filter (const sc_module_name& nm);
    void set_address(unsigned int address);
    void set_tag(unsigned int tag);
    unsigned int get_tag();
    
protected:
    void b_ctransport(tlm_generic_payload& trans, sc_time& delay);
    void b_itransport(tlm_generic_payload& trans, sc_time& delay);
};

#endif /* __TAG_FILTER_H__ */
