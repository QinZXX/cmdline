#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "cmd.h"

using namespace std;

extern string g_hostname;
extern string g_current_view;

typedef void (*cmd_fun_ptr)(void);
// TODO: handle input
class cmd_node_t {
public:
    string name;
    string des;
    cmd_fun_ptr func;
    // vector<string> input_list;

    cmd_node_t(){}

    cmd_node_t(const string& in_name, const string& in_des, cmd_fun_ptr in_func):
        name(in_name), des(in_des)
        {
            func = in_func;
        }
} ;

extern string g_ip;
vector<cmd_node_t*> g_cmd_top;

bool in_config_view()
{
    if ("(config)" == g_current_view) {
        return true;
    } else {
        return false;
    }
}

bool ip_str_is_valid(void) { // bool ip_str_is_valid(string ip_str)
    return true;
}



/***********************/
void show_func(void)
{
    printf("show func execute\r\n");
}

void help_func(void)
{
    // show current node level help info
    for (cmd_node_t *node : g_cmd_top) {
        printf("  %-10s\t%s\n",  node->name.c_str(), node->des.c_str());
    }
}

void exit_func(void)
{
    if (in_config_view()) {
        g_current_view = "";
    } else {
        free_all_cmd_modes();
        exit(0);
    }
}

void setip_func(void) // 没有输入
{
    if (in_config_view()) {
        if (ip_str_is_valid()) {
            printf("set ip %s success\r\n", g_ip.c_str());
        }
        printf("invalid ip\n", g_ip);
    } else {
        printf("error: not in config view\n");
    }
}

void enter_config_func(void)
{
    g_current_view = "(config)";
}

void enter_globalview_func(void)
{
    g_current_view = "";
}

void set_hostname_func(void)  // TODO: input
{
    static int count = 1;
    if (count % 2 == 1)
        g_hostname = "SG250";
    else 
        g_hostname = "SB6000";
    count++;
}

/**********************/
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
        printf("wrong input, please use \"help\" or \"?\"\n");
        return;
    }
    res->func();
}

void add_cmd_to_top(cmd_node_t* node) {
    g_cmd_top.push_back(node);
}


bool cmp_cmd_node(cmd_node_t* a, cmd_node_t* b)
{
    return (a->name < b->name);
}

void sort_cmd_tree() {
    std::sort(g_cmd_top.begin(), g_cmd_top.end(), cmp_cmd_node);
}

void construct_cmd_tree()
{
    cmd_node_t *node_show = new cmd_node_t("show", "show your ip", show_func);
    cmd_node_t *node_help = new cmd_node_t("help", "show help info", help_func);
    cmd_node_t *node_exit = new cmd_node_t("exit", "exit to last level", exit_func);
    cmd_node_t *node_setip = new cmd_node_t("setip", "set your ip", setip_func);
    cmd_node_t *node_config = new cmd_node_t("config", "enter configuration view", enter_config_func);
    cmd_node_t *node_end = new cmd_node_t("end", "return to global view", enter_globalview_func);
    cmd_node_t *node_hostname = new cmd_node_t("hostname", "return to global view", set_hostname_func);

    add_cmd_to_top(node_show);
    add_cmd_to_top(node_help);
    add_cmd_to_top(node_exit);
    add_cmd_to_top(node_setip);
    add_cmd_to_top(node_config);
    add_cmd_to_top(node_end);
    add_cmd_to_top(node_hostname);

    sort_cmd_tree();
}

void free_all_cmd_modes()
{
    for(cmd_node_t *node : g_cmd_top) {
        free(node);
        node = nullptr;
    }
}
