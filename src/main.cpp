//g++ projectclean.cpp exam.cpp -o myprogram to compile


#include"exam.h"
#include<bits/stdc++.h>
#include<list>
#include<cctype>
#include<string>
#include<cmath>
#include"json.hpp"
#include<fstream>

using json = nlohmann::json;
using namespace std;

int main(){
    srand(time(0));

    while(command != "x"){
        //signup
        if(command == "1"){
            command = "0";
            cout<<"Press 1 to signup as a teacher"<<endl<<"press 2 to signup as a student"<<endl<<"Press x to return to the main page"<<endl;
            getline(cin,command);
            // if(command == "x") goto main_page;
            while(command != "x" && command != "1" && command != "2"){
                cout<<"Please try again"<<endl;
                getline(cin,command);
            }
            if(command != "x") cout<<"Please fill out the required information.Press x to return to the main page."<<endl;
            //Teacher signup
            if(command == "1"){
                string firstName, lastName, id, course, password;
                teacher t;
                cout<<"First Name : ";
                getline(cin,firstName);
                cout<<"Last Name : ";
                getline(cin, lastName);
                t.set_name(firstName, lastName);
                cout<<"Courses(press 0 when done) : ";
                do{
                    getline(cin, course);
                    if(course == "0") break;
                    t.set_courses(course);
                }while(course != "0");
                cout<<"ID : ";
                getline(cin, id);
                t.set_id(id);
                cout<<"Enter a password : ";
                getline(cin, password);
                t.set_password(password);
                t.signup();
                //goto main_page;
            }
            //student signup 
            else if(command == "2"){
                student s;
                string firstName, lastName, id, major, password;
                cout<<"First Name : ";
                getline(cin,firstName);
                cout<<"Last Name : ";
                getline(cin, lastName);
                s.set_name(firstName, lastName);
                cout<<"Field of study : ";
                getline(cin,major);
                s.set_major(major);
                cout<<"ID : ";
                getline(cin, id);
                s.set_id(id);
                cout<<"Enter a password : ";
                getline(cin, password);
                s.set_password(password);
                s.signup();
            }
        }
        //login
        else if(command == "2"){
            cout<<"Press x to return to the main page.Press Enter to continue."<<endl;
            getline(cin, command);
            if(command != "x"){
                string inputID, inputPassword;
                cout<<"Enter your id : ";
                getline(cin,inputID);
                cout<<"Enter your password : ";
                getline(cin,inputPassword);

                fstream inFile("users.json");
                if (!inFile.good()){
                    cout<<"No users found!"<<endl;
                    command = "x";
                }
                else{
                    json data;
                    inFile >> data;
                    inFile.close();
                    bool user_exists = false;
                    for (const auto& user : data["users"]){
                        if (user["id"] == inputID){
                            user_exists = true;
                            if(user["password"] == inputPassword){
                                cout << "Login successful! Welcome, " << user["firstname"] << endl;
                                vector<string> user_examcodes;
                                if(user.contains("exam codes")){
                                    vector<string> user_examcodes = user["exam codes"];
                                }else{
                                    vector<string> user_examcodes = json::array();
                                }
                                if(user["occupation"] == "teacher"){
                                    teacher t;
                                    t.set_id(user["id"]);
                                    t.set_name(user["firstname"], user["lastname"]);
                                    t.set_examcodes(user_examcodes);
                                    t.teacher_panel();
                                }
                                else if(user["occupation"] == "student"){
                                    student s;
                                    s.set_id(user["id"]);
                                    s.set_name(user["firstname"], user["lastname"]);
                                    s.set_examcodes(user_examcodes);
                                    s.student_panel();
                                }
                            }
                        }
                        else if(user["id"] == inputID && user["password"] != inputPassword){
                            cout<<"Incorrect Password. Login Failed!"<<endl;
                            command = "x";
                            break;
                        }   
                    }
                    if(!user_exists){
                        cout<<"user not found. Login failed"<<endl;
                        command = "x";
                    }
                }
        }
    }
    if(command == "x" || command == "0"){
    cout<<"Press 1 to signup"<<endl<<"Press 2 to login"<<endl;
    getline(cin,command);
    while(command != "1" && command != "2" && command != "x"){
        cout<<"Please try again"<<endl;
        getline(cin,command);
    }  
        }
    }

}