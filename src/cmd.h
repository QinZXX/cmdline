#ifndef CMD_H
#define CMD_H

#include <string>
using std::string;

#define MAX_CHILD 26

void construct_cmd_tree();
void free_all_cmd_modes();
void parse_cmd(string cmd_name);

void help_func(void);


typedef void (*cmd_fun_ptr)(void);
// TODO: handle input
class cmd_node_t {
public:
    string name;
    string des;
    cmd_fun_ptr func;
    bool isEnd;
    cmd_node_t* child[MAX_CHILD];
    // vector<string> input_list;

    cmd_node_t();

    cmd_node_t(const string& in_name, const string& in_des, cmd_fun_ptr in_func, bool is_end = true);
};


#endif