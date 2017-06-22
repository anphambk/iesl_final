#ifndef __PE_TRANS_H__
#define __PE_TRANS_H__

#include "systemc.h"
#include "tlm.h"

//@
class pe_trans : public sc_object
{
public:
    typedef pe_trans tlm_payload_type;
    typedef tlm_phase tlm_phase_type;

    int address;
    char *data;
    int length;
    
    pe_trans() : data(NULL), length(0)
    {
    }

    ~pe_trans()
    {
        delete[] data;
    }

    pe_trans& operator= (const pe_trans& lsh)
    {
        address = lhs.address;
        data = lhs.data;
        length = lhs.length;
    }

    pe_trans& operator+ (const pe_trans& lsh)
    {
        assert((address == lsh.address) && (length == lhs.length));
        for(int i = 0; i < length; i++)
        {
            data[i] += lhs.data[i];
        }
    }
};

#endif /* __PE_TRANS_H__ */
