#include "common.h"
#include "top.h"

int sc_main(int argc, char **argv)
{
    char *verb;
    char *log_file;
    int show_log;

    map<string,sc_verbosity> parser;
    parser["SC_NONE"]=SC_NONE;
    parser["SC_LOW"]=SC_LOW;
    parser["SC_MEDIUM"]=SC_MEDIUM;
    parser["SC_HIGH"]=SC_HIGH;
    parser["SC_FULL"]=SC_FULL;
    parser["SC_DEBUG"]=SC_DEBUG;

    log_file  = sknobs_get_string("log_file", "sc.log");
    show_log = sknobs_get_value("show_log", 1);
    sc_report_handler::set_log_file_name(log_file);
    verb = sknobs_get_string("verbosity", "SC_LOW");
    string s = string(verb);
    sc_report_handler::set_verbosity_level(parser[s]);
    if(show_log == 0) {
        sc_report_handler::set_actions(SC_INFO, SC_LOG);
    }

    SC_REPORT_INFO_VERB("main", "Building .....", SC_FULL);
    top t("top");
    SC_REPORT_INFO_VERB("main", "Running .....", SC_FULL);
    sc_start();
    return 0;
}
