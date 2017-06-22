#include "ram.h"

//@
ram::ram (const sc_module_name& nm, unsigned int bus_width, unsigned int memory_size)
    : sc_module(nm)
    , m_bus_width(bus_width)
    , m_memory_size(memory_size)
    , port("port")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    assert(bus_width % 8 == 0);
    m_word_size = bus_width / 8;
    port.register_b_transport(this, &ram::b_transport);

    SC_REPORT_INFO_VERB(name(), format("Size=0x%X", memory_size), SC_LOW);
    if (memory_size) {
        m_memory = new unsigned char[memory_size];
    } else {
        m_memory = 0;
    }
    if (!m_memory) {
        cout << "TLM Memory Error: Cannot allocate memory with bounds 0x0:0x" << hex << memory_size << endl;
        abort();
    }
    memset(m_memory, 0, memory_size);
    m_power = sknobs_get_value(format("%s.power", name()), 0);
    m_total_accesses = 0;
}

//@
void ram::end_of_elaboration () 
{
    m_ports = port.size();
}

//@
void ram::end_of_simulation () 
{
    SC_REPORT_INFO_VERB(name(), format("Total accesses: %0lld", m_total_accesses), SC_LOW);
    SC_REPORT_INFO_VERB(name(), format("Total power consummed: %0lld", m_total_accesses*m_power), SC_LOW);
}

//@
void ram::b_transport (int id, tlm_generic_payload& trans, sc_time& delay)
{
    delay += SYS_CLK;
    operate(trans, delay);
}

//@
void ram::operate (tlm_generic_payload& trans, sc_time& delay)
{
    sc_dt::uint64    address = trans.get_address();     // memory address
    tlm::tlm_command command = trans.get_command();     // memory command
    unsigned char    *data   = trans.get_data_ptr();    // data pointer
    unsigned  int     length = trans.get_data_length(); // data length

    tlm_response_status response_status = check_address(trans)
        ? TLM_OK_RESPONSE
        : TLM_ADDRESS_ERROR_RESPONSE;

    //if (trans.get_byte_enable_ptr()) {
    //    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    //}
    //else if (trans.get_streaming_width() != trans.get_data_length()) {
    //    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    //}
    switch (command)
    {
        case TLM_WRITE_COMMAND:
        {
            if (response_status == TLM_OK_RESPONSE) {
                for (unsigned int i = 0; i < length; i++) {
                    m_memory[address++] = data[i];        // move the data to memory
                }
                delay += length / m_word_size * SYS_CLK;
            }
            break;
        }

        case TLM_READ_COMMAND:
        {
            if (response_status == TLM_OK_RESPONSE) {
                for (unsigned int i = 0; i < length; i++)
                {
                    data[i] = m_memory[address++];         // move the data from memory
                }
                delay += length / m_word_size * SYS_CLK;
            }
            break;
        }
        
        default:
        {
            trans.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
            break;
        }
    } // end switch
    trans.set_response_status(response_status);
    m_total_accesses += length / m_word_size;
}

//@
unsigned char* ram::get_mem_ptr(void)
{
    return m_memory;
}

//@
unsigned int ram::get_mem_size(void)
{
    return m_memory_size;
}

//@
bool ram::check_address (tlm_generic_payload& trans)
{
    uint64       address = trans.get_address();     // memory address
    unsigned int length  = trans.get_data_length(); // data length

    if  ( (address < 0) || ( (address + length-1) > m_memory_size ) )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//@
void ram::initialize (unsigned int address, char* file)
{
    ifstream fh (file);
    float data;
    unsigned char* cdata;
    unsigned int size = 0;
    
    cdata = (unsigned char*)(&data);
    fh >> data;  // You must attempt to read info prior to an eof() test
    while(!fh.eof())
    {
        m_memory[address + 4 * size    ] = *(cdata    );
        m_memory[address + 4 * size + 1] = *(cdata + 1);
        m_memory[address + 4 * size + 2] = *(cdata + 2);
        m_memory[address + 4 * size + 3] = *(cdata + 3);
        fh >> data;
        size++;
    }
    fh.close();
}

//@
void ram::dump (unsigned int address, unsigned int length)
{
    char msg [100];
    float data;
    unsigned char* cdata;
    
    cdata = (unsigned char*)(&data);
    sprintf(msg, "Dumping RAM data from address=0x%X to address=0x%d", address, address + 4 * length - 1);
    SC_REPORT_INFO_VERB(name(), msg, SC_NONE);
    for (int i = 0; i < length; i++)
    {
        *(cdata    ) = m_memory[address + 4 * i    ];
        *(cdata + 1) = m_memory[address + 4 * i + 1];
        *(cdata + 2) = m_memory[address + 4 * i + 2];
        *(cdata + 3) = m_memory[address + 4 * i + 3];
        sprintf(msg, "address=%d, data=%f", address + 4 * i, data);
        SC_REPORT_INFO_VERB(name(), msg, SC_NONE);
    }
}
