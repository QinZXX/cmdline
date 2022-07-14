#include "cmd.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

extern string g_hostname;
extern string g_current_view;

cmd_node_t::cmd_node_t()
{
    name = "";
    des = "";
    func = NULL;
    isEnd = false;

    for (int i = 0; i < MAX_CHILD; ++i) {
        child[i] = nullptr;
    }
}

cmd_node_t::cmd_node_t(const string& in_name, const string& in_des, cmd_fun_ptr in_func, bool is_end)
    : name(in_name)
    , des(in_des)
    , func(in_func)
    , isEnd(is_end)
{
}

extern string g_ip;
// vector<cmd_node_t*> g_cmd_top;
cmd_node_t* g_cmd_root = new cmd_node_t();

bool in_config_view()
{
    if ("(config)" == g_current_view) {
        return true;
    } else {
        return false;
    }
}

bool ip_str_is_valid(void)
{ // bool ip_str_is_valid(string ip_str)
    return true;
}

/***********************/
void show_func(void)
{
    printf("show func execute\r\n");
}

void show_all_cmd(cmd_node_t* cur)
{
    // cmd_node_t* cur = g_cmd_root;
    if (nullptr == cur) {
        return;
    }

    for (int i = 0; i < MAX_CHILD; ++i) {
        if (nullptr != cur->child[i]) {
            if (true == cur->child[i]->isEnd) {
                printf("  %-10s\t%s\n", cur->child[i]->name.c_str(), cur->des.c_str());
            }
            show_all_cmd(cur->child[i]);
        }
    }
}

void help_func(void) // show all cmd name and des
{
    // show current node level help info
    show_all_cmd(g_cmd_root);
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

void set_hostname_func(void) // TODO: input
{
    static int count = 1;
    if (count % 2 == 1)
        g_hostname = "SG250";
    else
        g_hostname = "SB6000";
    count++;
}

void exec_extern_func(void)
{
    cout << "Please input the extern cmdline which you wanna execute:" << endl;
    string cmd;
    FILE* fp = nullptr;
    char read_str[512] = { 0 };

    cin >> cmd;
    // getline(cin, cmd);

    if (nullptr == (fp = popen(cmd.c_str(), "r"))) {
        cout << "popen failed" << endl;
        return;
    }
    while (fgets(read_str, sizeof(read_str), fp)) {
        cout << read_str << endl;
    }
    pclose(fp);
}

/**********************/
cmd_node_t* find_cmd_node(string in_name)
{
    cmd_node_t* cur = g_cmd_root;
    cmd_node_t* cur_child;

    for (int i = 0; i < in_name.size(); ++i) {
        cur_child = cur->child[in_name[i] - 'a'];
        if (nullptr == cur_child) {
            return nullptr; // 没有某个字母，说明没有该cmd_name
        }
        cur = cur_child;
    }

    return cur;
}

// src_node is not a complete cmd node, in which cmd name is not complete
void auto_complete_cmd(cmd_node_t* src_node)
{
    cmd_node_t* cur = src_node;

    for (int i = 0; i < MAX_CHILD; ++i) {
        if (nullptr != cur->child[i]) {
            if (true == cur->child[i]->isEnd) {
                // cout << "  autocom: " << cur->child[i]->name << endl;
                cout << "  " << cur->child[i]->name << endl;
                auto_complete_cmd(cur->child[i]);
            } else { // 还存在节点，但此节点不是单词末尾
                auto_complete_cmd(cur->child[i]); // 递归处理孩子节点
            }
        } else { // 说明没有i节点了，继续搜索 i+1 child节点
            // continue;
        }
    }
}

void parse_cmd(string cmd_name)
{
    // check input
    for (int i = 0; i < cmd_name.size(); ++i) {
        if (cmd_name[i] < 'a' || cmd_name[i] > 'z') {
            cout << "illegal input, cmd line can only include [a-z]!" << endl;
            return;
        }
    }

    cmd_node_t* res;
    res = find_cmd_node(cmd_name);
    if (nullptr == res) {
        printf("wrong input, please use \"help\" or \"?\"\n");
        return;
    } else {
        if (true == res->isEnd) {
            // cout << "whole cmd word find! cmd name:  " << res->name << endl;
            res->func();
        } else {
            cout << "Please input a complete cmd. Which cmd you mean?" << endl;
            auto_complete_cmd(res);
        }
    }
}

void add_cmd_to_top(cmd_node_t* node)
{
    cmd_node_t* cur = g_cmd_root;
    cmd_node_t* cur_child;
    string s = node->name;

    cout << "add top cmd: " << s << endl;

    for (int i = 0; i < s.size(); ++i) {
        cur_child = cur->child[s[i] - 'a'];

        if (nullptr == cur_child) {
            cur_child = new cmd_node_t(); // 如果节点不存在，则新建;并且更新子节点的string
            cur_child->name = cur->name + string(1, (char)s[i]);
            cur->child[s[i] - 'a'] = cur_child;
        } else { // 如果节点已存在，那么不需要新建节点，string也不需要更新
            // cout << "  node exist" << endl;
        }

        // 移动到下一个节点，继续添加，直到这个字符串的全部字符都被加入
        cur = cur_child;
    }
    cur->isEnd = true;
    cur->des = node->des;
    cur->func = node->func;
    // cmd name has bee added
}

bool cmp_cmd_node(cmd_node_t* a, cmd_node_t* b)
{
    return (a->name < b->name);
}

void sort_cmd_tree()
{
    // std::sort(g_cmd_top.begin(), g_cmd_top.end(), cmp_cmd_node);
}

void construct_cmd_tree()
{
    cmd_node_t* node_show = new cmd_node_t("show", "show your ip", show_func);
    cmd_node_t* node_help = new cmd_node_t("help", "show help info", help_func);
    cmd_node_t* node_exit = new cmd_node_t("exit", "exit to last level", exit_func);
    cmd_node_t* node_setip = new cmd_node_t("setip", "set your ip", setip_func);
    cmd_node_t* node_config = new cmd_node_t("config", "enter configuration view", enter_config_func);
    cmd_node_t* node_end = new cmd_node_t("end", "return to global view", enter_globalview_func);
    cmd_node_t* node_hostname = new cmd_node_t("hostname", "return to global view", set_hostname_func);
    cmd_node_t* node_exec = new cmd_node_t("exec", "execute extern cmd program", exec_extern_func);

    add_cmd_to_top(node_show);
    add_cmd_to_top(node_help);
    add_cmd_to_top(node_exit);
    add_cmd_to_top(node_setip);
    add_cmd_to_top(node_config);
    add_cmd_to_top(node_end);
    add_cmd_to_top(node_hostname);
    add_cmd_to_top(node_exec);

    sort_cmd_tree();
}

void free_all_cmd_modes()
{
    /*
    for (cmd_node_t* node : g_cmd_top) {
        free(node);
        node = nullptr;
    }
    */
    // TODO
}
