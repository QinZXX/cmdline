#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "cmd.h"

using namespace std;

string g_hostname;
string g_current_view;
string g_ip = "";

int main()
{
    string in;
    
    construct_cmd_tree();

    g_hostname = "SG6000";
    g_current_view = "";
BEGIN:
    cout << g_hostname <<g_current_view << "# ";
    cin >> in; // TODO: get line and split by white space
    if (!in.empty()) {
        if (in == "?") {
            help_func();
            goto BEGIN;
        }
        parse_cmd(in);
        goto BEGIN;
    } else { // 直接回车
        goto BEGIN;
    }

    free_all_cmd_modes();
    return 0;
}