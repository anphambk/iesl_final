#ifndef __PE_H__
#define __PE_H__

#include "common.h"

class pe : public sc_module
{
public:
    static const unsigned int IBUFFER_SIZE = 100;
    static const unsigned int FBUFFER_SIZE = 100;
    static const unsigned int PBUFFER_SIZE = 100;

    //simple_target_socket< pe > csocket;
    simple_target_socket< pe > isocket;
    simple_target_socket< pe > fsocket;
    simple_target_socket< pe > psocket;
    simple_initiator_socket< pe > opsocket;

protected:
    SC_HAS_PROCESS(pe);
    unsigned char m_idata[IBUFFER_SIZE];
    unsigned char m_fdata[FBUFFER_SIZE];
    unsigned char m_pdata[PBUFFER_SIZE];
    tlm_generic_payload m_image;
    tlm_generic_payload m_filter;
    tlm_generic_payload m_psum;
    
    unsigned int m_address;
    cnn_type_t m_type = CNN_CONV;
    bool m_active;
    sc_event opsum_e;

public:
    pe (const sc_module_name& nm);
    void set_address (unsigned int address);
    void set_type (cnn_type_t t);
    cnn_type_t get_type ();
    void set_active (bool a);
    bool get_active ();
    
protected:
    void b_ctransport (tlm_generic_payload& trans, sc_time& delay);
    void b_itransport (tlm_generic_payload& trans, sc_time& delay);
    void b_ftransport (tlm_generic_payload& trans, sc_time& delay);
    void b_ptransport (tlm_generic_payload& trans, sc_time& delay);
   
    void send_opsum();
    void accumulate (tlm_generic_payload& trans, sc_time& delay);
    void operate (sc_time& delay);
    void conv (sc_time& delay);
    void pool (sc_time& delay);
    void fc (sc_time& delay);
};

#endif /* __PE_H__ */
