#include "broadcast.h"

//@
broadcast::broadcast (const sc_module_name& nm)
    : sc_module(nm)
    , isocket("isocket")
    , osocket("osocket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    isocket.register_b_transport(this, &broadcast::b_transport);
}

//@
void broadcast::end_of_elaboration()
{
    m_size = osocket.size();
}

//@
void broadcast::b_transport (tlm_generic_payload& trans, sc_time& delay)
{
    sc_time odelay[m_size];
    sc_time max_delay;
    for (int i = 0; i < m_size; i++)
    {
        osocket[i]->b_transport(trans, odelay[i]);
        if (max_delay < odelay[i])
        {
            max_delay = odelay[i];
        }
    }
    delay += max_delay; 
}
