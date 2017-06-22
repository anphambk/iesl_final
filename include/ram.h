#ifndef __RAM_H__
#define __RAM_H__

#include "common.h"

class ram : public sc_module
{
public:
    multi_passthrough_target_socket < ram > port;
    typedef enum
    {
          RAM_WRITE_FIRST
        , RAM_READ_FIRST
        , RAM_NO_CHANGE
    } ram_mode_t;
    
protected:
    unsigned int m_ports;
    unsigned int m_memory_size;
    unsigned int m_bus_width;
    unsigned int m_word_size;
    unsigned char* m_memory;
    ram_mode_t m_mode = RAM_WRITE_FIRST;
    // power estimation
    unsigned int m_power; // estimated power per access
    unsigned int m_total_accesses;

public:
    ram (const sc_module_name& nm, unsigned int bus_width, unsigned int memory_size);
    unsigned char* get_mem_ptr(void);
    unsigned int get_mem_size();
    void initialize (unsigned int address, char* file);
    void dump (unsigned int address, unsigned int length);

protected:
    void end_of_elaboration ();
    void end_of_simulation ();
    void b_transport (int id, tlm_generic_payload& trans, sc_time& delay);
    
    void operate (tlm_generic_payload& trans, sc_time& delay);
    bool check_address (tlm_generic_payload& trans);
};

#endif /* __RAM_H__ */
