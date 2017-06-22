#include "pe.h"

//@
pe::pe (const sc_module_name& nm)
    : sc_module(nm)
    //, csocket("csocket")
    , isocket("isocket")
    , fsocket("fsocket")
    , psocket("psocket")
    , opsocket("opsocket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    //csocket.register_b_transport(this, &pe::b_ctransport);
    isocket.register_b_transport(this, &pe::b_itransport);
    fsocket.register_b_transport(this, &pe::b_ftransport);
    psocket.register_b_transport(this, &pe::b_ptransport);
    m_image.set_data_ptr(m_idata);
    m_filter.set_data_ptr(m_fdata);
    m_psum.set_data_ptr(m_pdata);
    m_active = sknobs_get_value(format("%s.active", name()), 0);
    SC_THREAD(send_opsum);
}

//@
void pe::b_ctransport (tlm_generic_payload& trans, sc_time& delay)
{
    unsigned char* dptr = trans.get_data_ptr();

    if (trans.get_address() == m_address)
    {
        switch (trans.get_command())
        {
            case TLM_WRITE_COMMAND:
                //memcpy(m_type, dptr, sizeof(int));
                trans.set_response_status(TLM_OK_RESPONSE);
                break;
            case TLM_READ_COMMAND:
                trans.set_data_length(sizeof(int));
                //memcpy(dptr, m_type, sizeof(int));
                trans.set_response_status(TLM_OK_RESPONSE);
                break;
            default: ;
        }
    }
    delay += SYS_CLK;
}

//@
void pe::b_itransport (tlm_generic_payload& trans, sc_time& delay)
{
    if (m_active == 1)
    {
        m_image.deep_copy_from(trans);
        operate(delay);
    }
    trans.set_response_status(TLM_OK_RESPONSE);
    delay += SYS_CLK;
}

//@
void pe::b_ftransport (tlm_generic_payload& trans, sc_time& delay)
{
    if (m_active == 1)
    {
        m_filter.deep_copy_from(trans);
        operate(delay);
    }
    trans.set_response_status(TLM_OK_RESPONSE);
    delay += SYS_CLK;
}

//@
void pe::b_ptransport (tlm_generic_payload& trans, sc_time& delay)
{
    if (m_active == 1)
    {
        accumulate(trans, delay);
        //m_psum.set_response_status(TLM_INCOMPLETE_RESPONSE);
        //opsocket->b_transport(m_psum, delay);
        opsum_e.notify(delay);
        trans.set_response_status(TLM_OK_RESPONSE);
    }
    else
    {
        opsocket->b_transport(trans, delay);
    }
    delay += SYS_CLK;
}

//@
void pe::send_opsum()
{
    sc_time delay;
    while(true)
    {
        wait(opsum_e);
        delay = SC_ZERO_TIME;
        opsocket->b_transport(m_psum, delay);
        wait(delay);
    }
}

//@
void pe::operate(sc_time& delay)
{
    switch (m_type)
    {
        case CNN_CONV:
            if (m_image.get_data_length() && m_filter.get_data_length())
            {
                conv(delay);
            }
            break;
        case CNN_POOL:
            pool(delay);
            break;
        case CNN_FC:
            fc(delay);
            break;
        default:
            SC_REPORT_FATAL(name(), "Illegal CNN type");
    }
}

//@
void pe::accumulate(tlm_generic_payload& trans, sc_time& delay)
{
    float* lhs = (float*)m_psum.get_data_ptr();
    float* rhs = (float*)trans.get_data_ptr();
    unsigned int len = m_psum.get_data_length() / sizeof(float);
    
    assert(m_psum.get_data_length() == trans.get_data_length());
    switch (m_type)
    {
        case CNN_CONV:
            for (int i = 0; i < len; i++)
            {
                *(lhs + i) += *(rhs + i);
            }
            break;
        case CNN_POOL:
            for (int i = 0; i < len; i++)
            {
                if (*(lhs + i) < *(rhs + i))
                {
                    *(lhs + i) = *(rhs + i);
                }
            }
            break;
        case CNN_FC:
            break;
        default:
            SC_REPORT_FATAL(name(), "Illegal CNN type");
    }
    delay += len * SYS_CLK;
}

//@
void pe::conv(sc_time& delay)
{
    float* idata = (float*)m_image.get_data_ptr();
    float* fdata = (float*)m_filter.get_data_ptr();
    float* pdata = (float*)m_psum.get_data_ptr();
    unsigned int ilen = m_image.get_data_length() / sizeof(float);
    unsigned int flen = m_filter.get_data_length() / sizeof(float);
    unsigned int plen = ilen - flen + (flen % 2);

    m_psum.set_data_length(plen * sizeof(float));
    for (int i = 0; i < plen; i++)
    {
        *(pdata + i) = 0;
        for (int j = 0; j < flen; j++)
        {
            *(pdata + i) += *(fdata + j) * *(idata + i + j);
            delay += SYS_CLK;
        }
    }
}

//@
void pe::pool(sc_time& delay)
{
    float* idata = (float*)m_image.get_data_ptr();
    float* pdata = (float*)m_psum.get_data_ptr();
    unsigned int ilen = m_image.get_data_length() / sizeof(float);
    unsigned int plen = (ilen / 2) + (ilen % 2);

    m_psum.set_data_length(plen * sizeof(float));
    for (int i = 0; i < plen; i++)
    {
        if ((2 * i + 1) < ilen)
        {
            *(pdata + i) = max(*(idata + 2 * i), *(idata + 2 * i + 1));
        }
        else
        {
            *(pdata + i) = *(idata + 2 * i);
        }
        delay += SYS_CLK;
    }
}

//@
void pe::set_address (unsigned int address)
{
    m_address = address;
}

/* DEPRICATED */
//@
void pe::fc(sc_time& delay)
{
}

//@
void pe::set_type(cnn_type_t t)
{
    m_type = t;
}

//@
cnn_type_t pe::get_type()
{
    return m_type;
}

//@
void pe::set_active(bool a)
{
    m_active = a;
}

//@
bool pe::get_active()
{
    return m_active;
}

