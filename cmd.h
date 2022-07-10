#ifndef CMD_H
#define CMD_H

#include <string>
using std::string;

void construct_cmd_tree();
void free_all_cmd_modes();
void parse_cmd(string cmd_name);

void help_func(void);

#endif