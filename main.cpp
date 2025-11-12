#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class Utility
{
public:
    static vector<string> split(string s, char d)
    {
        vector<string> out;
        string cur = "";
        for (char c : s)
        {
            if (c == d)
            {
                out.push_back(cur);
                cur = "";
            }
            else
            {
                cur.push_back(c);
            }
        }
        out.push_back(cur);
        return out;
    }

    static void pause()
    {
        cout << "Press enter to continue\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

class User
{
public:
    string username;
    string password;
    string gender;
    string preference;
    string bio;

    User() {}
    User(string u, string p, string g, string pref, string b)
    {
        username = u;
        password = p;
        gender = g;
        preference = pref;
        bio = b;
    }

    bool save()
    {
        ofstream f("data/users.txt", ios::app);
        if (!f)
            return false;
        f << username << '|' << password << '|' << gender << '|' << preference << '|' << bio << "\n";
        f.close();
        return true;
    }

    static vector<User> loadAll()
    {
        vector<User> out;
        ifstream f("data/users.txt");
        string line;
        while (getline(f, line))
        {
            if (line.size() == 0)
                continue;
            vector<string> parts = Utility::split(line, '|');
            if (parts.size() >= 5)
            {
                User u(parts[0], parts[1], parts[2], parts[3], parts[4]);
                out.push_back(u);
            }
        }
        f.close();
        return out;
    }

    static bool login(string u, string p)
    {
        vector<User> all = loadAll();
        for (auto &x : all)
        {
            if (x.username == u && x.password == p)
                return true;
        }
        return false;
    }

    void display()
    {
        cout << "Username: " << username << "\n";
        cout << "Gender: " << gender << "\n";
        cout << "Preference: " << preference << "\n";
        cout << "Bio: " << bio << "\n";
    }
};

// Forward declarations for other cpp files
void matchMenu(const User &me);
void chatMenu(const string &me);

User findUser(string uname)
{  
    vector<User> all = User::loadAll();
    for (auto &x : all)
        if (x.username == uname)
            return x;
    return User();
}

void registerFlow()
{
    string u, p, g, pref, b;
    cout << "Enter username: ";
    getline(cin, u);
    cout << "Enter password: ";
    getline(cin, p);
    cout << "Enter gender (M/F/O): ";
    getline(cin, g);
    cout << "Enter preference (M/F/O): ";
    getline(cin, pref);
    cout << "Enter a short bio: ";
    getline(cin, b);
    User newu(u, p, g, pref, b);
    bool ok = newu.save();
    if (ok)
        cout << "Registered " << u << "\n";
    else
        cout << "Error saving user\n";
    Utility::pause();
}

void loginFlow()
{
    cout << "Username: ";
    string u;
    getline(cin, u);
    cout << "Password: ";
    string p;
    getline(cin, p);
    bool ok = User::login(u, p);
    if (ok)
    {
        cout << "Welcome " << u << "\n";
        User me = findUser(u);
        while (true)
        {
            cout << "1 View Profile\n2 Find Matches\n3 Chat\n4 Logout\nChoose: ";
            string c;
            getline(cin, c);
            if (c == "1")
            {
                me.display();
                Utility::pause();
            }
            else if (c == "2")
            {
                matchMenu(me);
            }
            else if (c == "3")
            {
                chatMenu(me.username);
            }
            else if (c == "4")
                break;
            else
                cout << "Invalid\n";
        }
    }
    else
    {
        cout << "Wrong creds\n";
        Utility::pause();
    }
}

int main()
{
    while (true)
    {
        cout << "1 Register\n2 Login\n3 Exit\nChoose: ";
        string c;
        getline(cin, c);
        if (c == "1")
            registerFlow();
        else if (c == "2")
            loginFlow();
        else if (c == "3")
            break;
        else
            cout << "Invalid\n";
    }
    return 0;
}

// ============== MATCHMAKER FUNCTIONS ==============

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

// ============== CHAT FUNCTIONS ==============

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
    if (n <= 0 || n > static_cast<int>(ppl.size()))
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
