#include "format.h"

char* gp_to_string(tlm::tlm_generic_payload& trans)
{
    unsigned int len = trans.get_data_length()/sizeof(float);
    unsigned int slen;
    char* msg = new char [1024];

    sprintf(msg, "transaction: address=%0d, length=%0d, data=", trans.get_address(), len);
    for(int i = 0; i < len; i++)
    {
        slen = sprintf(msg, "%s%f_", msg, *((float*)trans.get_data_ptr() + i));
        if (i >= 10) {
            slen = sprintf(msg, "%s .........", msg);
            return msg;
        }
    }
    return msg;
}


//@
char* format (const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    char* buf = vformat (fmt, ap);
    va_end (ap);
    return buf;
}

//@
char* vformat (const char *fmt, va_list ap)
{
    // Allocate a buffer on the stack that's big enough for us almost
    // all the time.
    unsigned int size = 1024;
    char* buf = new char[size];

    // Try to vsnprintf into our buffer.
    va_list apcopy;
    va_copy (apcopy, ap);
    int needed = vsnprintf (&buf[0], size, fmt, ap);
    // NB. On Windows, vsnprintf returns -1 if the string didn't fit the
    // buffer.  On Linux & OSX, it returns the length it would have needed.

    if (needed <= size && needed >= 0) {
        // It fit fine the first time, we're done.
        return &buf[0];
    } else {
        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So do a malloc of the right size and try again.
        // This doesn't happen very often if we chose our initial size
        // well.
        std::vector <char> buf;
        size = needed;
        buf.resize (size);
        needed = vsnprintf (&buf[0], size, fmt, apcopy);
        return &buf[0];
    }
}
