#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "cmd.h"

using namespace std;


int main()
{
    string in;
    
    construct_cmd_tree();

BEGIN:
    cout << "SG6000 config: ";
    cin >> in;
    if (!in.empty()) {
        parse_cmd(in);
        goto BEGIN;
    }

    return 0;
}