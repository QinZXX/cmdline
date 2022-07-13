#include <iostream>
#include <string>

using namespace std;

const int max_char = 26;

struct trienode {
    trienode()
    {
        for (int i = 0; i < max_char; ++i) {
            child[i] = nullptr;
        }
        isEnd = false;
        cur_str = "";
    }

    bool isEnd; // 如果为一个单词的末尾，则把它置为true
    trienode* child[max_char];
    string cur_str;
};

void build_tree(const string& s, trienode* root)
{
    cout << "build tree, string is: " << s << endl;
    trienode* cur = root;
    trienode* cur_child;

    for (int i = 0; i < s.size(); ++i) {
        cur_child = cur->child[s[i] - 'a'];

        if (nullptr == cur_child) {
            cur_child = new trienode(); // 如果节点不存在，则新建;并且更新子节点的string
            cur_child->cur_str = cur->cur_str + string(1, (char)s[i]);
            cur->child[ s[i] - 'a'] = cur_child;
        } else {//// 如果节点已存在，那么不需要新建节点，string也不需要更新
            //cout << "  node exist" << endl;
        }

        // 移动到下一个节点，继续添加，直到这个字符串的全部字符都被加入
        cur = cur_child;
    }
    cur->isEnd = true;
}

void insert_node();

trienode* search_tree(const string& s, trienode* root)
{
    trienode* cur = root;
    trienode* cur_child;

    cout << "search string: " << s << endl;
    for (int i = 0; i < s.size(); ++i) {
        cur_child = cur->child[s[i] - 'a'];
        if (nullptr == cur_child) {
            return nullptr; // 没有某个字母，说明没有该单词
        }
        // 否则，说明有该单词。继续查找剩余字母，直到全部字母查完
        cur = cur_child;
    }

    // 全部字母查完了都有，此时还没return，则返回当前节点
    return cur;

    // 后续需要自己处理判断
    // 1.如果是 单词bug，查找bu，是bug的前缀，则此时 isEnd为false，说明没有此单词，只是一个前缀而已
    // 2. isEnd = true，说明是一个完整单词
}

// 把全部的 含有目前前缀的那啥单词(isEnd = true)都找出来
// vector<trienode*>
void auto_complete(trienode* src_node)
{
    trienode* cur = src_node;

    for (int i = 0; i < max_char; ++i) {
        if (nullptr != cur->child[i]) {
            if (true == cur->child[i]->isEnd) {
                cout << "  autocom: " << cur->child[i]->cur_str << endl;
                auto_complete(cur->child[i]); 
            } else { // 还存在节点，但此节点不是单词末尾
                auto_complete(cur->child[i]); // 递归处理孩子节点
            }
        } else { // 说明没有i节点了，继续搜索 i+1 child节点
            //continue;
        }
    }
}

int main()
{
    cout << "hello!" << endl;
    trienode* zxroot = new trienode(); // 新建一个空root节点哦
    string cmd1 = "show";
    string cmd2 = "config";
    string cmd3 = "setip";
    string cmd4 = "setipifa";
    string cmd5 = "setipifb";
    string cmd6 = "showif";

    build_tree(cmd1, zxroot);
    build_tree(cmd2, zxroot);
    build_tree(cmd3, zxroot);
    build_tree(cmd4, zxroot);
    build_tree(cmd5, zxroot);
    build_tree(cmd6, zxroot);
    // build_tree(cmd1, zxroot);

BEGIN:
    cout << "input the world which you want to search:" << endl;
    string input;
    cin >> input;

    trienode* res1 = search_tree(input, zxroot);

    if (nullptr != res1) {
        cout << "I find words, maybe prefix or whole word" << endl;
        if (true == res1->isEnd) {
            cout << "whole word find! res->cur_str: "<< res1->cur_str << endl;
            goto BEGIN;
        } else {
            cout << "prefix find! res1->cur_str: "<< res1->cur_str << endl;
            goto AUTOCOM;
        }
    } else {
        cout << "no such word or prefix" << endl;
        goto BEGIN;
    }

AUTOCOM: // need to find whole word
    auto_complete(res1);
    goto BEGIN;

END:
    // TODO:release dynamic memery
    // destroy_tree(zxroot);

    return 0;
}
