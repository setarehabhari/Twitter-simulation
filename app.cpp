#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include "json.hpp"
#include "httplib.h"
#include <chrono>
#include <random>
using namespace httplib;
using namespace std;
using json = nlohmann::json;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
minstd_rand0 generator (seed);

string logged_in_user_username = "" ;

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str);
    string tok;

    while(getline(ss,tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

bool regis(Client cli)
{
    string input;
    cout<<"enter fields with ','in between"<<endl;
    cin>>input;
    input = "INSERT INTO users VALUES "+ input ;

    if (auto res = cli.Post("/query", input, "text/plain")){
        if (res->status == 200){
            string id = res->body;
            if (id == "-1") {
                cout << "no account with this information exists" << endl;
                return false;
            }
            else
            {
                cout<<"account created"<<endl;
                return true;
            }
        }
    }
}
bool log(Client cli)
{
    string input;
    cout<<"enter you username"<<endl;
    cin>>input;
    string query = "SELECT FROM users WHERE username=\"" + input +"\"";
    string pass;
    cout<<"enter password"<<endl;
    cin >> pass;
    if (auto res = cli.Post("/query", query , "text/plain")){
        if (res->status == 200){
            json result = json::parse(res->body);
            if (result[0]["id"]=="-1")
            {
                cout<<"username does not exist"<<endl;
                logged_in_user_username = "" ;
                return false;
            }
            if(result[0]["password"]==pass) {
                logged_in_user_username = result[0]["username"];
                return true;
            }
            else
                cout<<"password is wrong"<<endl;
        }
    }
    cout << "connection error " << endl ;
    return false;
}
string get_current_time(){
    time_t t = time(0);
    tm* now = localtime(&t);
    string mon = to_string(now->tm_mon+1);
    if (mon.size() == 1)
        mon = "0" + mon;
    string day = to_string(now->tm_mday);
    if (day.size() == 1)
        day = "0" + day;
    return to_string(now->tm_year - 100)+"/"+mon+"/"+day ;
}
void share(Client cli)
{
    string time = get_current_time() ;
    string input;
    cout<<"enter your tweet"<<endl;
    cin>>input;
    string query = "INSERT INTO tweet VALUES "+to_string(generator()) +","+ input +","+logged_in_user_username+","+time;///enter time
    if (auto res = cli.Post("/query", query, "text/plain")) {
        if (res->status == 200) {
            json result = json::parse(res->body);
            cout << result["id"] << endl ;
        }
    }
}
void view(Client cli)
{
    string query = "SELECT FROM tweet";
    if (auto res = cli.Post("/query", query , "text/plain")){
        if (res->status == 200){
            json result = json::parse(res->body);
            cout << result.dump(4) << endl ;
            return ;
        }
    }
    cout << "connection error " << endl ;
    return ;
}
void like (Client cli)
{
    string tweet_id ;
    cout << "pls enter tweet id" << endl ;
    cin >> tweet_id ;
    string query = "INSERT INTO likes VALUES "+ tweet_id + "," + logged_in_user_username ;
    if (auto res = cli.Post("/query", query , "text/plain")){
        if (res->status == 200){
            cout << "liked!" << endl ;
            return ;
        }
    }
    cout << "connection error " << endl ;
    return ;
}
void view_likes (Client cli)
{
    string tweet_id ;
    cout << "pls enter tweet id" << endl ;
    cin >> tweet_id ;
    string query = "SELECT FROM likes WHERE tweet_id=\""+ tweet_id + "\"" ;
    if (auto res = cli.Post("/query", query , "text/plain")){
        if (res->status == 200){
            json result = json::parse(res->body);
            for (auto& element : result) {
                cout << element["username"] << endl;
            }
            return ;
        }
    }
    cout << "connection error " << endl ;
    return ;
}
void ret(Client cli)
{
    string tweet_id ;
    cout << "pls enter tweet id" << endl ;
    cin >> tweet_id ;
    string query = "SELECT FROM tweet WHERE tweet_id=\""+ tweet_id + "\"" ;
    if (auto res = cli.Post("/query", query , "text/plain")){
        if (res->status == 200){
            cout << res->body << endl ;
            json result = json::parse(res->body);
            string time_string = get_current_time() ;
            string query2 = "INSERT INTO tweet VALUES "+
                    to_string(generator() + 100) + "," +
                    to_string(result[0]["text"]) + "," +
                    logged_in_user_username + "," +
                    time_string ;
            if (auto res2 = cli.Post("/query", query2 , "text/plain")){
                if (res2->status == 200){
                    json result2 = json::parse(res2->body);
                    cout << result2["id"] << endl ;
                }
            }
            return ;
        }
    }
    cout << "connection error " << endl ;
    return;
}

int main()
{
    Client cli("localhost", 8080);
//    temp(cli);
    cout<<"for logging in enter 1, for registering enter 2"<<endl;
    int reg;
    cin >> reg;
    bool con;
    string username;
    if(reg==2) {
        if(regis(cli));
            con = log(cli);
    }
    if (reg == 1)
        con = log(cli);
    if (con)
    {
        while (true)
        {
            cout<<"enter 1 to share a tweet" << endl;
            cout<<"enter 2 to view all tweets" << endl;
            cout<<"enter 3 to like a tweet" << endl;
            cout<<"enter 4 to view likes of a tweet" << endl ;
            cout<<"enter 5 to retweet a tweets" << endl;
            cout<<"enter 6 to quit the program "<<endl;
            int status;
            cin >> status;
            if (status == 6)
                break;
            else if (status ==1 )
                share(cli);
            else if (status == 2)
                view(cli);
            else if (status == 3)
                like(cli);
            else if (status == 4)
                view_likes(cli);
            else if (status == 5)
                ret(cli) ;

        }
    }
}