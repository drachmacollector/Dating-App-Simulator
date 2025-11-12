#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string>

using namespace std;

void saveMatch(string a, string b)
{
    ofstream f("data/matches.txt", ios::app);
    if (f)
        f << a << '|' << b << "\n";
    f.close();
}

vector<pair<string, string>> loadMatches()
{
    vector<pair<string, string>> out;
    ifstream f("data/matches.txt");
    string line;
    while (getline(f, line))
    {
        if (line.size() == 0)
            continue;
        size_t pos = line.find('|');
        if (pos != string::npos)
        {
            string a = line.substr(0, pos);
            string b = line.substr(pos + 1);
            out.push_back({a, b});
        }
    }
    f.close();
    return out;
}

vector<string> findMatchesFor(const User &u)
{
    vector<User> all = User::loadAll();
    vector<string> out;
    for (auto &x : all)
    {
        if (x.username == u.username)
            continue;
        if (u.preference == x.gender)
            out.push_back(x.username);
    }
    return out;
}

void matchMenu(const User &me)
{
    vector<string> matches = findMatchesFor(me);
    if (matches.empty())
    {
        cout << "No matches found\n";
        Utility::pause();
        return;
    }
    for (size_t i = 0; i < matches.size(); i++)
        cout << i + 1 << ". " << matches[i] << "\n";
    cout << "Enter number to match or 0 to cancel: ";
    string s;
    getline(cin, s);
    int n = stoi(s);
    if (n > 0 && n <= matches.size())
    {
        saveMatch(me.username, matches[n - 1]);
        cout << "Matched with " << matches[n - 1] << "\n";
    }
    else
        cout << "Cancelled\n";
    Utility::pause();
}
