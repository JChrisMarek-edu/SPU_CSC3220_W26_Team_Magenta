#include <iostream>
#include <ctime>
#include <string>
#include <sqlite3.h> //make sure you install sqlite through linux/ubuntu or something else
//or this will not work. also, if any of the code doesn't work for you, let nate know.
using namespace std;

//    g++ main.cpp -lsqlite3 -o program
//just leave this here

struct teams { //i'm pretty sure we don't need these structs if we're using sqlite
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

string getInput(){
    string input = "";
    cin>>input;
        for (int i = 0; i < input.size(); i++){ //convert string to lowercase
           input.at(i) = tolower(input.at(i)); 
            
        }
        return input;
}

static int callback(void* unused, int argc, char** argv, char** azColName) { //Chatgpt made this method
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": "
             << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "---------------------" << endl;
    return 0;
}


int main(){

sqlite3* db;

int rc = sqlite3_open("okr.db", &db);

if (rc) {
    cout << "Can't open database: "<< sqlite3_errmsg(db)<<endl;
    return 1;
}
    cout << "Database opened successfully!"<<endl;

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    //create team table
    const char* createTeamTable =
            "CREATE TABLE IF NOT EXISTS team ("
            "TeamID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "TeamName TEXT NOT NULL"
            ");";

            sqlite3_exec(db, createTeamTable, nullptr, nullptr, nullptr);
    
            //create employee table
    const char* createEmployeeTable =
            "CREATE TABLE IF NOT EXISTS employee ("
            "EmployeeID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "FirstName TEXT NOT NULL,"
            "LastName TEXT NOT NULL,"
            "EMAIL TEXT UNIQUE NOT NULL,"
            "Role TEXT,"
            "HireDate TEXT,"
            "TeamID INTEGER,"
            "FOREIGN KEY (TeamID) REFERENCES team(TeamID)"
            ");";

            sqlite3_exec(db, createEmployeeTable, nullptr, nullptr, nullptr);
            
            //create Objectives Table
            const char* createObjectivesTable =
            "CREATE TABLE IF NOT EXISTS objectives ("
            "ObjectiveID INTEGER PRIMARY KEY,"
            "Title TEXT NOT NULL,"
            "Description TEXT,"
            "StartDate TEXT,"
            "EndDate TEXT,"
            "Status TEXT,"
            "TeamID INTEGER NOT NULL,"
            "OwnerEmployeeID INTEGER,"
            "FOREIGN KEY (TeamID) REFERENCES team(TeamID),"
            "FOREIGN KEY (OwnerEmployeeID) REFERENCES employee(EmployeeID)"
            ");";

            sqlite3_exec(db, createObjectivesTable, nullptr, nullptr, nullptr);
            
            //Create Key Results Table
            const char* createKeyResultsTable =
            "CREATE TABLE IF NOT EXISTS key_results ("
            "KeyResultID INTEGER PRIMARY KEY,"
            "ObjectiveID INTEGER NOT NULL,"
            "Title TEXT NOT NULL,"
            "MetricName TEXT,"
            "StartValue REAL,"
            "CurrentValue REAL,"
            "TargetValue REAL,"
            "MaxValue REAL,"
            "Unit TEXT,"
            "StartDate TEXT,"
            "DueDate TEXT,"
            "Status TEXT,"
            "FOREIGN KEY (ObjectiveID) REFERENCES objectives(ObjectiveID)"
            ");";

            sqlite3_exec(db, createKeyResultsTable, nullptr, nullptr, nullptr);






    cout<<"Welcome to OK Results."<<endl;
    string input = "";

    while (!(input == "q" || input == "quit")){
        input = getInput();
        //debug code
        //cout<<input<<endl;

        if (input == "help" || input == "/help"){
            //help command
            cout<<"Commands:"<<endl;
            cout<<"---------------------------"<<endl;
            cout<<"Create: Add a new team, employee, Objective, or Key Result"<<endl;
            cout<<"Delete: remove some or all data from a database table of your choice"<<endl;
            cout<<"List: Read all data from a database table of your choice"<<endl;
            cout<<"Help: Opens this menu"<<endl;
            cout<<"Update: not implemented yet"<<endl;
            cout<<""<<endl;
            cout<<""<<endl;
        }

        else if(input == "create" || input == "new"){
            cout<<"What type of object would you like to create?"<<endl;
            cout<<"OKR Object types are: Team, Employee, Key Result, and Objective"<<endl;
            input = getInput();
            
            if (input == "employee"){//mods, doxx this guy
                cout<<"For all fields, don't include any spaces in your input."<<endl; //it causes a silly bug i dont know how to fix we can ignore for now

                string firstName = "";
                string lastName = "";
                string email = ""; 
                string role = "";
                int teamID = -1;

                cout << "First Name: ";
                cin >> firstName;

                cout << "Last Name: ";
                cin >> lastName;

                cout << "Email: ";
                cin >> email;

                cout << "Role: ";
                cin >> role;

                cout << "Team ID: ";
                cin >> teamID;

                string addEmployee =
                    "INSERT INTO employee "
                    "(FirstName, LastName, Email, Role, TeamID) VALUES ('" +
                firstName + "', '" + lastName + "', '" + email + "', '" +
                role + "', " + to_string(teamID) + ");";

                sqlite3_exec(db, addEmployee.c_str(), nullptr, nullptr, nullptr);

                cout << firstName + " " + lastName +" created." << endl;
            }else if (input == "team"){
        
            string teamName = "";
            cout << "Enter Team Name: ";
            cin.ignore();
            getline(cin, teamName);


            
            string sql = "INSERT INTO team (TeamName) VALUES ('" + teamName + "');";
            



            char* errMsg = nullptr;

            sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg); //check if sqlite has an error, and prints if it does

            if (errMsg) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            }







            }else if (input == "objective"){
                //create objective
            }else if (input == "key result"){
                //create objective
            }else{
                cout<<"inproper object type."<<endl;
            }


        }
        else if (input == "delete" || input == "kill") {

            cout << "Delete which object? choose from team, employee, objective, or key_result" << endl;
             input = getInput();

            if (input == "team") {
                cout << "Enter Team ID: ";
                int id;
                cin >> id;

                string sql =
                    "DELETE FROM team WHERE TeamID = " + to_string(id) + ";";

                sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

                cout << "Team deleted." << endl;
            }else if  (input == "employee"){
                //FIXME implement employee deleting
            }else if  (input == "key_result"){
                //FIXME implement key result deleting
            }else if  (input == "objective"){
                //FIXME implement objective deleting
            }

        }else if (input == "list" || input == "view") { //view data

            cout << "What do you want to list? (team, employee, objective)" << endl;
            input = getInput();

            if (input == "team") {
                cout<<"Viewing teams"<<endl;
                const char* sql = "SELECT * FROM team;";
                sqlite3_exec(db, sql, callback, nullptr, nullptr);
            }





        }else if (input == "update" || input == "edit") {
            //add this

        }else{
            cout<<"Unidentified input. type help for a list of commands."<<endl;
        }

        

    }



}


string printDate(int myDate){
    
    return "printDate is currently not programmed";


}



