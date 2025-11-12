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

// Include other implementation units into the single translation unit.
// This avoids duplicate-definition linker errors when building all .cpp files separately.
#include "Matchmaker.cpp"
#include "Chat.cpp"
