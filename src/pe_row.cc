# include "pe_row.h"

//@
pe_row:: pe_row (const sc_module_name& nm, unsigned int size)
    : sc_module(nm)
    , m_size(size)
    , isocket("isocket")
    , fsocket("fsocket")
    , psocket("psocket")
    , opsocket("opsocket")
    , ibroadcast("ibroadcast")
    , fbroadcast("fbroadcast")
    , itags("itags")
    , ftags("ftags")
    , elements("elements")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    psocket.init(size);
    opsocket.init(size);
    itags.init(size);
    ftags.init(size);
    elements.init(size);

    isocket.bind(ibroadcast.isocket);
    fsocket.bind(fbroadcast.isocket);
    for(int i = 0; i < size; i++)
    {
        ibroadcast.osocket.bind(itags[i].isocket);
        itags[i].osocket.bind(elements[i].isocket);
        fbroadcast.osocket.bind(ftags[i].isocket);
        ftags[i].osocket.bind(elements[i].fsocket);
        psocket[i].bind(elements[i].psocket);
        elements[i].opsocket.bind(opsocket[i]);
    }
}
