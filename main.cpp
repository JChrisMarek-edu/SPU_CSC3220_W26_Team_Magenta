#include <iostream>
#include <ctime>
#include <string>
using namespace std;

struct teams {
    int teamID;
    string teamName;
};

struct employee {
    int employeeID;
    string fName;
    string lName;
    string email;
    string role;
    string hireDate; //change to date object
    int teamID;
};

struct objective {
    int objectiveID;
    string title;
    string description;
    string startDate;
    string endDate;
    string status;
    int teamID;
    int ownerEmployeeID;
};
struct date{
    int month;
    int day;
    int year;
};

int main(){

    cout<<"Welcome to OK Results."<<endl;
    string input = "";

    while (!(input == "q" || input == "quit")){
        cin>>input;
        for (int i = 0; i < input.size(); i++){ //convert entire string to lowercase
            input.at(i) = std::tolower(input.at(i)); //i know there's a better way to do it, i'm too tired to figure it out
            // it should also probably trim leading and ending whitespace, we can do that later though
        }


        if (input == "help" || input == "/help"){
            //help command
        }

        else if(input == "create" || input == "new"){
            cout<<"What type of object would you like to create?"<<endl;
            cout<<"OKR Object types are: Team, Employee, and Objective"<<endl;
            cin>>input;
            if (input == ("employee")){
                //create employee 
            }else if (input == "team"){
                //create team
            }else if (input == "objective"){
                //create objective
            }else{
                cout<<"inproper object type."<<endl;
            }
        }
        else if (input == "delete" || input == "kill"){
            cout<<"enter the name of the object you wish to delete"<<endl;
        }


    }



}


string printDate(date myDate){
    return std::to_string(myDate.month) + "/" + std::to_string(myDate.day) + "/" + std::to_string(myDate.year);


}