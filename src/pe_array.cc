#include "pe_array.h"

//@
pe_array::pe_array (const sc_module_name& nm, unsigned int hsize, unsigned int vsize)
    : sc_module(nm)
    , m_hsize(hsize)
    , m_vsize(vsize)
    , isocket("isocket")
    , fsocket("fsocket")
    , psocket("psocket")
    , opsocket("opsocket")
    , ibroadcast("ibroadcast")
    , fbroadcast("fbroadcast")
    , rows("rows")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    psocket.init(hsize);
    opsocket.init(hsize);
    rows.init(vsize, sc_bind(&pe_array::create_row, this, sc_unnamed::_1, sc_unnamed::_2));

    isocket.bind(ibroadcast.isocket);
    fsocket.bind(fbroadcast.isocket);

    for (int i = 0; i < vsize; i++)
    {
        ibroadcast.osocket.bind(rows[i].isocket);
        fbroadcast.osocket.bind(rows[i].fsocket);
    }
    for (int i = 0; i < hsize; i++)
    {
        psocket[i].bind(rows[0].psocket[i]);
        if(vsize > 1)
        {
            for(int j = 0; j < vsize - 1; j++)
            {
                rows[j].opsocket[i].bind(rows[j+1].psocket[i]);
            }
        }
        rows[vsize-1].opsocket[i].bind(opsocket[i]);
    }
}

//@
pe_row* pe_array::create_row(const sc_module_name& nm, size_t)
{
    return new pe_row(nm, m_hsize);
}
