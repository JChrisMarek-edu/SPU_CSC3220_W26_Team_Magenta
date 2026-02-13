#include <iostream>
#include <ctime>
using namespace std;

struct teams {
    int teamID;
    string teamName;
}

struct employee {
    int employeeID;
    string fName;
    string lName;
    string email;
    string role;
    string hireDate; //change to date object
    int teamID;
}

struct objective {
    int objectiveID;
    string title;
    string description;
    string startDate;
    string endDate;
    string status;
    int teamID;
    int ownerEmployeeID;
}

int main(){

cout<<"Welcome to OK Results."<<endl;
string input = "";

while (!(input == "q" || input == "quit")){
cin>>input;

if (input == "help" || input == "/help"){
    //help command
}



}



}