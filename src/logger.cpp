#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

void log(const string &str, const string &arg = "")
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cout << buf << endl;
}

void logerror(const string &str, const string &arg = "")
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << buf << endl;
}