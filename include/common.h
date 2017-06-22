#ifndef __COMMON_H__
#define __COMMON_H__

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <deque>
//#include <

#include <systemc.h>
#include <tlm.h>

#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <tlm_utils/peq_with_get.h>
#include <tlm_utils/peq_with_cb_and_phase.h>

#include "sknobs.h"
#include "format.h"

using namespace std;
using namespace sc_dt;
using namespace tlm;
using namespace tlm_utils;

static const sc_time SYS_CLK = sc_time(10, SC_PS);
typedef enum {CNN_CONV, CNN_POOL, CNN_FC} cnn_type_t;

#endif /* __COMMON_H__ */
