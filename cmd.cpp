#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;


typedef void (*cmd_fun_ptr)(void);
class cmd_node_t {
public:
    string name;
    string des;
    cmd_fun_ptr func;

    cmd_node_t(){}

    cmd_node_t(const string& in_name, const string& in_des, cmd_fun_ptr in_func):
        name(in_name), des(in_des)
        {
            func = in_func;
        }
} ;

int g_ip = 0;
vector<cmd_node_t*> g_cmd_top;

void show_func(void)
{
    printf("show func execute\r\n");
}

void help_func(void)
{
    printf("help func execute\r\n");
}

void exit_func(void)
{
    printf("exit func execute\r\n");
    exit(0);
}

void setip_func(void) // 没有输入
{
    printf("set ip %d success\r\n", g_ip);
}

cmd_node_t* find_cmd_node(string in_name)
{
    cmd_node_t *res = nullptr;
    for(cmd_node_t *node : g_cmd_top) {
        if (in_name == node->name) {
            return node;
        }
    }
    return res;
}

void parse_cmd(string cmd_name)
{
    //find cmd_node by name;
    cmd_node_t* res;
    res = find_cmd_node(cmd_name);
    if (nullptr == res) {
        printf("wrong input\n");
        return;
    }
    res->func();
}

void add_cmd_to_top(cmd_node_t* node) {
    g_cmd_top.push_back(node);
}

void construct_cmd_tree()
{
    cmd_node_t *node_show = new cmd_node_t("show", "show your ip", show_func);
    cmd_node_t *node_help = new cmd_node_t("help", "show help info", help_func);
    cmd_node_t *node_exit = new cmd_node_t("exit", "exit cmd now", exit_func);
    cmd_node_t *node_setip = new cmd_node_t("setip", "set your ip", setip_func);

    add_cmd_to_top(node_show);
    add_cmd_to_top(node_help);
    add_cmd_to_top(node_exit);
    add_cmd_to_top(node_setip);
}
