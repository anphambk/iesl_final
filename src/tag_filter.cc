#include "tag_filter.h"

//@
tag_filter::tag_filter (const sc_module_name& nm)
    : sc_module(nm)
    , m_tag(0)
    //, csocket("csocket")
    , isocket("isocket")
    , osocket("osocket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    //csocket.register_b_transport(this, &tag_filter::b_ctransport);
    isocket.register_b_transport(this, &tag_filter::b_itransport);
    m_tag = sknobs_get_value(format("%s.tag", name()), 0);
}

//@
void tag_filter::b_ctransport (tlm_generic_payload& trans, sc_time& delay)
{
    if (trans.get_address() == m_address)
    {
        switch (trans.get_command())
        {
            case TLM_WRITE_COMMAND:
                //memcpy(m_tag, trans.get_data_ptr(), sizeof(int));
                trans.set_response_status(TLM_OK_RESPONSE);
                break;
            case TLM_READ_COMMAND:
                trans.set_data_length(sizeof(int));
                //memcpy(trans.get_data_ptr(), m_tag, sizeof(int));
                trans.set_response_status(TLM_OK_RESPONSE);
                break;
            default: ;
        }
    }
    delay += SYS_CLK;
}

//@
void tag_filter::b_itransport (tlm_generic_payload& trans, sc_time& delay)
{
    if(trans.get_address() == m_tag)
    {
        osocket->b_transport(trans, delay);
    }
    delay += SYS_CLK;
}

//@
void tag_filter::set_address(unsigned int address)
{
    m_address = address;
}

/* DEPRICATED */
//@
void tag_filter::set_tag(unsigned int tag)
{
    m_tag = tag;
}

//@
unsigned int tag_filter::get_tag()
{
    return m_tag;
}
