#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <string>
#include <cstdarg>
#include <vector>

#include <tlm.h>

char* gp_to_string(tlm::tlm_generic_payload& trans);
char* format (const char *fmt, ...);
char* vformat (const char *fmt, va_list ap);

#endif /* __FORMAT_H__ */
