//hello darkness my old friend
//okkkkk
#include <iostream>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "httplib.h"
#include "json.hpp"
#include <chrono>
#include <random>

using namespace std;
using namespace httplib;
using json = nlohmann::json;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
minstd_rand0 generator (seed);

string text;

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str);
    string tok;

    while(getline(ss,tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

bool check_unique(string field_name,string field_value,string table_name){
    ifstream File;
    File.open(table_name);
    string fields;
    string arr[100][100];
    int k = 0;
    int size = 0;
    while (getline(File, fields)) {
        vector<string> field = split(fields, ' ');
        if (k == 0)
            size = field.size();
        for (int i = 0; i < size; ++i) {
            arr[k][i] = field[i];
        }
        ++k;
    }
    for (int i = 1; i < 100; ++i) {
        if(arr[i][0]==field_value) {
            cout << "this field name already exists and it should be unique" << endl;
            return false;
        }
    }
    return true;
}

bool check(string field_value, string table_name,int num)
{
    ifstream Myfile;
    Myfile.open(table_name,ios_base::app | ios_base::out);
    string field_name ;
    getline(Myfile,field_name);
    if(field_name.empty())
        return true;

    vector<string> vec = split(field_name,' ');
    field_name = vec[num];
    Myfile.close();

    ifstream File;
    File.open("schema.txt",ios_base::app | ios_base::out);
    if (File.is_open()) {
        string str;
        bool table = false;
        while (getline(File, str)) {
           if(table==true)
            {
                vector<string> line = split(str,' ');
                if (line[0]== field_name)
                {
                    bool x = true;
                    if (line[1] == "UNIQUE" )
                        x = check_unique(field_name,field_value,table_name);
                    if (x==false)
                        return false;
                    if(str.find('(') != string::npos)
                    {
                        vector <string>type = split(line[line.size()-1],'(');
                         if(type[0]=="INTEGER")
                        {
                            for (int i = 0; i < field_value.size(); ++i) {
                                if(isdigit(field_value[i])==false) {
                                    cout << "input for this field should be an integer" << endl;
                                    return false;
                                }
                            }
                        }
                        else if(type[0]=="TIMESTAMP")
                        {
                            if (isdigit(field_value[0])&&isdigit(field_value[1])&&field_value[2]=='/')
                            {
                                if(isdigit(field_value[3])&&isdigit(field_value[4])&&field_value[5]=='/')
                                {
                                    if(isdigit(field_value[6])&&isdigit(field_value[7]))
                                        return true;
                                    else {
                                        cout << "input for this field should be a timestamp" << endl;
                                        return false;
                                    }
                                }else {
                                    cout << "input for this field should be a timestamp" << endl;
                                    return false;
                                }
                            }else {
                                cout << "input for this field should be a timestamp" << endl;
                                return false;
                            }
                            if (field_value.size()!=8) {
                                cout << "input for this field should be a timestamp" << endl;
                                return false;
                            }
                        }
                        else if(type[0]=="BOOLEAN")
                        {
                            if(field_value!="true" && field_value!= "false") {
                                cout << "input for this field should be a boolean" << endl;
                                return false;
                            }
                        }
                        else if (type[0]!="CHAR") {
                            cout << "this type of field does not exist" << endl;
                            return false;
                        }
                        string allowed_size = type[1].substr(0,type[1].size()-1);
                        if (field_value.size() > stoi(allowed_size)) {
                            cout << "the size of the input is bigger than allowed" << endl;
                            return false;
                        }
                    }
                    else{
                        if(line[1]=="INTEGER")
                        {
                            for (int i = 0; i < field_value.size(); ++i) {
                                if(isdigit(field_value[i])==false) {
                                    cout << "input for this field should be an integer" << endl;
                                    return false;
                                }
                            }
                        }
                        else if(line[1]=="TIMESTAMP")
                        {
                            if (isdigit(field_value[0])&&isdigit(field_value[1])&&field_value[2]=='/')
                            {
                                if(isdigit(field_value[3])&&isdigit(field_value[4])&&field_value[5]=='/')
                                {
                                    if(isdigit(field_value[6])&&isdigit(field_value[7]))
                                        return true;
                                    else {
                                        cout << "input for this field should be a timestamp" << endl;
                                        return false;
                                    }
                                }else {
                                    cout << "input for this field should be a timestamp" << endl;
                                    return false;
                                }
                            }else {
                                cout << "input for this field should be a timestamp" << endl;
                                return false;
                            }
                            if (field_value.size()!=8) {
                                cout << "input for this field should be a timestamp" << endl;
                                return false;
                            }
                        }
                        else if(line[1]=="BOOLEAN")
                        {
                            if(field_value!="true" && field_value!= "false") {
                                cout << "input for this field should be a boolean" << endl;
                                return false;
                            }
                        }
                        else if (line[1]!="CHAR") {
                            cout << "this type of field does not exist" << endl;
                            return false;
                        }

                    }
                    return true;
                }
            }
            if(table_name==str+".txt")
                table = true;
        }
    }
    File.close();
}

void create_file(string txtstr) {
    string name;
    vector <string> table_field = split(txtstr,'\n');
    name = table_field[0] + ".txt";
    ifstream chek;
    chek.open(name);
    if (chek.fail()) {
        ofstream MyFile(name);
        for (int i = 1; i < table_field.size(); ++i) {
            vector<string> field = split(table_field[i], ' ');
            MyFile << field[0] + ' ';
        }
        MyFile <<'\n';
        MyFile.close();
    }
}

int ins(vector <string> word)//stat = 0
{
    ofstream file;
    string name = word[2]+".txt";
    file.open(name,ios_base::app | ios_base::out);
    if (file.fail()) {
        cout<<"said file does not exist"<<endl;
        return -1;
    }
    else{
        vector <string> values = split(word[4],',');
        string ans;
        int k = 0;
        for (int i = 0; i < values.size(); ++i) {
            string str =  values[i] ;
            cout << str << endl ;
            cout << check(str,name,i) << endl ;
            if (check(str,name,i)==1) {
                str += " ";
                ans += str;
            }
            else
                k=1;
        }
        if(k == 0)
        {
            int x = generator();
            file << ans + to_string(x) + "\n";
            return x ;
        }
        return -1;

    }
}

bool check_space(string input){
    for (int i=0 ;i < input.size(); i++){
        if (input[i] != ' ' && input[i] != '\n'){
            return false ;
        }
    }
    return true ;
}

void dlt(vector <string> word) //stat =1
{
    ifstream file;
    string name = word[2] + ".txt";
    file.open(name,ios_base::app | ios_base::out);
    if (file.fail()) {
        cout << "said file does not exist" << endl;
        return;
    } else {
        string fields;
        string arr[100][100];
        int k = 0;
        int size = 0;
        int numofrows=0;

        while (getline(file, fields)) {
            
            ++numofrows;
            if (check_space(fields)){
                continue ;
            }
            vector<string> field = split(fields, ' ');
            
            if (k == 0)
                size = field.size();
            for (int i = 0; i < size+1; ++i) {
                arr[k][i] = field[i];
            }
            ++k;
        }
        for (int i = 4; i < word.size(); i = i + 2) {
            string field_name = word[i].substr(0, word[i].find("="));
            string field_value = word[i].substr(word[i].find("=") + 1);
            field_value = field_value.substr(1,field_value.size()-2);
            int m = -1;
            for (int j = 0; j < size; ++j) {
                if (arr[0][j] == field_name) {
                    m = j;
                    break;
                }
            }
            if (m == -1) {
                cout << "field name does not exist in this table" << endl;
                return;
            }
            for (int j = 0; j < 100; ++j) {
                //cout<<arr[j][m]<<' '<<field_value<<endl;
                if (arr[j][m] == field_value) {
                    for (int l = 0; l < size+1; ++l) {
                        arr[j][l] = "";
                    }
                }
            }

        }
        remove(name.c_str());
        ofstream MyFile(name);
        string new_file;
        for (int i = 0; i < numofrows; ++i) {
            for (int j = 0; j < size+1; ++j) {
                new_file += arr[i][j]+' ';
            }
            new_file += '\n';
        }
        MyFile << new_file;
    }
    return;
}

void upd(vector<string> word)//stat = 2
{
    ifstream file;
    string name = word[1] + ".txt";
    file.open(name,ios_base::app | ios_base::out);
    if (file.fail()) {
        cout << "said file does not exist" << endl;
        return;
    } else {
        string fields;
        string arr[100][100];
        int k = 0;
        int size = 0;
        int numofrows=0;
        while (getline(file, fields)) {
            ++numofrows;
            vector<string> field = split(fields, ' ');
            if (k == 0)
                size = field.size();
            for (int i = 0; i < size+1; ++i) {
                arr[k][i] = field[i];
            }
            ++k;
        }
        int val = 0;
        for (int i = 0; i < word.size(); ++i) {
            if (word[i] == "VALUES")
                val = i;
        }
        for (int i = 3; i < val; i = i + 2) {
            string field_name = word[i].substr(0, word[i].find("="));
            string field_value = word[i].substr(word[i].find("=") + 1);
            field_value = field_value.substr(1,field_value.size()-2);
            int m = -1;
            for (int j = 0; j < size; ++j) {
                if (arr[0][j] == field_name) {
                    m = j;
                    break;
                }
            }
            if (m == -1) {
                cout << "field name does not exist in this table" << endl;
                return;
            }
            for (int j = 1; j < 100; ++j) {
                if (arr[j][m] == field_value) {
                    vector<string> fields_new = split(word[val+1], ',');
                    for (int l = 0; l < fields_new.size(); ++l) {
                        if(!check(fields_new[l],name,l))
                            return;
                    }
                    for (int l = 0; l < size; ++l) {
                        arr[j][l] = fields_new[l];
                    }
                }
            }

        }
        remove(name.c_str());
        ofstream MyFile(name);
        string new_file;
        for (int i = 0; i < numofrows; ++i) {
            for (int j = 0; j < size+1; ++j) {
                new_file += arr[i][j]+' ';
            }
            new_file += '\n';
        }
        MyFile << new_file;
    }
}

json sel(vector<string> word)//stat = 3
{
    ifstream file;
    string name = word[2] + ".txt";
    file.open(name,ios_base::app | ios_base::out);
    if (file.fail()) {
        cout << "said file does not exist" << endl;
        return nullptr;
    } else {
        string fields;
        string arr[100][100];
        int k = 0;
        int size = 0;
        while (getline(file, fields)) {
            vector<string> field = split(fields, ' ');
            if (k == 0)
                size = field.size();
            for (int i = 0; i < field.size(); ++i) {
                arr[k][i] = field[i];
            }
            ++k;
        }
        json result ;
        if (word.size() == 3){
            for (int j = 1; j < 100; ++j) {
                json row ;
                if (arr[j][0] != "" && arr[j][0] != " " && arr[j][0] != "\n") {
                    for (int l = 0; l < size + 1; ++l) {
                        string namef = "";
                        if (l == size)
                            namef = "id";
                        else
                            namef = arr[0][l];
                        row[namef] = arr[j][l];
                    }
                }
                if (!row.is_null())
                    result.push_back(row);
            }
            return result;
        }
        for (int i = 4; i < word.size(); i = i + 2) {
            string field_name = word[i].substr(0, word[i].find("="));
            string field_value = word[i].substr(word[i].find("=") + 1);
            field_value = field_value.substr(1,field_value.size()-2);
            int m = -1;
            for (int j = 0; j < size; ++j) {
                if (arr[0][j] == field_name) {
                    m = j;
                    break;
                }
            }
            if (m == -1) {
                cout << "field name does not exist in this table" << endl;
                return nullptr;
            }
            for (int j = 0; j < 100; ++j) {
                //cout<<arr[j][m]<<' '<<field_value<<endl;
                json row ;
                if (arr[j][m] == field_value) {
                    for (int l = 0; l < size+1; ++l) {
                        string name = "" ;
                        if (l == size)
                            name = "id" ;
                        else
                            name = arr[0][l];
                        row[name] = arr[j][l] ;
                    }
                    
                }
                if (!row.is_null())
                    result.push_back(row);
            }

        }
        return result;
    }
}


int main() {
    ifstream File;
    ifstream Program;

    File.open("schema.txt");
    if (File.is_open()) {
        string str;
        string table;
        while (getline(File, str)) {
            if (str == "") {
                create_file(table);
                table = "";
            } else
                table += str + '\n';
            text += str + '\n';
        }
    }
    File.close();

    Server svr;
    svr.set_keep_alive_max_count(20);


    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.Post("/query", [&](const auto& req, auto& res) {
        string input_string = req.body;
        vector<string> word;
        word = split(input_string, ' ');
        if (word[0] == "INSERT"){
            int id = ins(word);
            cout << id << endl ;
            json data ={
                {"id", id}
            };
            res.set_content(data.dump(), "text/plain");
            cout << "done insert" << endl ;
        }
        else if (word[0] == "DELETE"){
            dlt(word);
            json data ={
                {"msg", "done"}
            };
            res.set_content(data.dump(), "text/plain");
            cout << "done delete" << endl ;
        }
        else if (word[0] == "UPDATE"){
            upd(word);
            json data ={
                {"msg", "done"}
            };
            res.set_content(data.dump(), "text/plain");
            cout << "done update" << endl;
        }
        else if (word[0] == "SELECT"){
            json data = sel(word);
            res.set_content(data.dump(), "text/plain");
            cout << "done select" << endl;
        }

    });

    svr.listen("localhost", 8080);

}