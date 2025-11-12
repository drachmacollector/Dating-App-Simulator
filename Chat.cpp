#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string>

using namespace std;

vector<string> loadChat(string a, string b)
{
    vector<string> out;
    ifstream f("data/chats.txt");
    string line;
    while (getline(f, line))
    {
        if (line.size() == 0)
            continue;
        vector<string> parts = Utility::split(line, '|');
        if (parts.size() >= 3)
        {
            if ((parts[0] == a && parts[1] == b) || (parts[0] == b && parts[1] == a))
                out.push_back(parts[0] + ": " + parts[2]);
        }
    }
    f.close();
    return out;
}

void saveMessage(string a, string b, string msg)
{
    ofstream f("data/chats.txt", ios::app);
    if (f)
        f << a << '|' << b << '|' << msg << "\n";
    f.close();
}

void chatMenu(const string &me)
{
    ifstream fm("data/matches.txt");
    vector<string> ppl;
    string line;
    while (getline(fm, line))
    {
        size_t pos = line.find('|');
        if (pos != string::npos)
        {
            string a = line.substr(0, pos);
            string b = line.substr(pos + 1);
            if (a == me)
                ppl.push_back(b);
            else if (b == me)
                ppl.push_back(a);
        }
    }
    fm.close();
    if (ppl.empty())
    {
        cout << "No matches to chat\n";
        Utility::pause();
        return;
    }
    for (size_t i = 0; i < ppl.size(); i++)
        cout << i + 1 << ". " << ppl[i] << "\n";
    cout << "Choose: ";
    string s;
    getline(cin, s);
    int n = stoi(s);
    if (n <= 0 || n > ppl.size())
        return;
    string partner = ppl[n - 1];
    cout << "Chatting with " << partner << "\n";
    vector<string> old = loadChat(me, partner);
    for (auto &m : old)
        cout << m << "\n";
    cout << "Type 'exit' to stop\n";
    while (true)
    {
        string msg;
        getline(cin, msg);
        if (msg == "exit")
            break;
        saveMessage(me, partner, msg);
        cout << me << ": " << msg << "\n";
        string reply = "ok";
        saveMessage(partner, me, reply);
        cout << partner << ": " << reply << "\n";
    }
}
