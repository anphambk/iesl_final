
#ifndef __PROTOCOLS_H__
#define __PROTOCOLS_H__

#include "common.h"

//@ Protocol for DMA
struct dma_payload
{
    //
    unsigned char direction;
    unsigned int length;
    unsigned int address0;
    unsigned int address1;
    
    //these methods are just dummy implementation in order to compile
    bool has_mm() {return false;}
    void set_mm(bool mm) { return; }
    void set_auto_extension(void *x) { return; }
    //we currently do not support these methods, so we have to emulate the functionality.
    void acquire() { return; }
    void release() { return; }
    int get_ref_count() { return 1; }
};

union dma_payload_u
{
    dma_payload s;
    unsigned char dptr [sizeof(dma_payload)];
};

struct dma_protocol
{
    typedef dma_payload tlm_payload_type;
    typedef tlm_phase tlm_phase_type;
};

//@ Protocol for CPU instruction
struct instruction_payload
{
    //
    cnn_type_t cnn_type;
    unsigned int image_address;
    unsigned int image_width;
    unsigned int image_height;
    unsigned int image_depth;
    unsigned int filter_address;
    unsigned int filter_size;
    unsigned int filter_stride;
    unsigned int filter_number;
    unsigned int psum_address;
    unsigned char psum_reuse;
    
    //these methods are just dummy implementation in order to compile
    bool has_mm() {return false;}
    void set_mm(bool mm) { return; }
    void set_auto_extension(void *x) { return; }
    //we currently do not support these methods, so we have to emulate the functionality.
    void acquire() { return; }
    void release() { return; }
    int get_ref_count() { return 1; }
};

union instruction_payload_u
{
    instruction_payload s;
    unsigned char dptr [sizeof(instruction_payload)];
};

struct instruction_protocol
{
    typedef instruction_payload tlm_payload_type;
    typedef tlm_phase tlm_phase_type;
};

#endif /* __PROTOCOLS_H__ */
