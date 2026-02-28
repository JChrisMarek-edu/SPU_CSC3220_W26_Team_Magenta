#include <iostream>
#include <string>
#include <sqlite3.h> //make sure you install sqlite through linux/ubuntu or something else
//or this will not work. also, if any of the code doesn't work for you, let nate know.
using namespace std;

//    g++ main.cpp -lsqlite3 -o program
//just leave this here, so i can copy-paste compile quickly


string getInput(){
    string input = "";
    getline(cin, input);
        for (int i = 0; i < input.size(); i++){ //convert string to lowercase
           input.at(i) = tolower(input.at(i)); 
            if (input.at(i) == ' '){
                input.at(i) = '_';
            }
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

if (sqlite3_open("okr.db", &db) != SQLITE_OK) {
    std::cerr << "Cannot open database\n";
    return 1;
}

sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    cout << "Database opened successfully!"<<endl;

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    //create team table
    const char* createTeamTable =
            "CREATE TABLE IF NOT EXISTS team ("
            "TeamID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "TeamName TEXT NOT NULL"
            ");";

            char* errMsg = nullptr;

            int rc = sqlite3_exec(db, createTeamTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 
            
    
            //create employee table
    const char* createEmployeeTable =
            "CREATE TABLE IF NOT EXISTS employee ("
            "EmployeeID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "FirstName TEXT NOT NULL,"
            "LastName TEXT NOT NULL,"
            "Email TEXT UNIQUE NOT NULL,"
            "Role TEXT,"
            "HireDate TEXT,"
            "TeamID INTEGER,"
            "FOREIGN KEY (TeamID) REFERENCES team(TeamID)"
            ");";


            char* errMsg = nullptr;

            int rc = sqlite3_exec(db, createEmployeeTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 
            
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

            char* errMsg = nullptr;

            int rc = sqlite3_exec(db, createObjectivesTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 
            
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

            char* errMsg = nullptr;

            int rc = sqlite3_exec(db, createKeyResultsTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 


            const char* createEmployeeKeyResultsTable =
            "CREATE TABLE IF NOT EXISTS employee_key_result ("
            "EmployeeID INTEGER NOT NULL,"
            "KeyResultID INTEGER NOT NULL,"
            "AssignedDate TEXT NOT NULL,"
            "Weight REAL,"
            "Status TEXT,"
            "PRIMARY KEY (EmployeeID, KeyResultID),"
            "FOREIGN KEY (EmployeeID) REFERENCES employee(EmployeeID),"
            "FOREIGN KEY (KeyResultID) REFERENCES key_results(KeyResultID)"
            ");";

            char* errMsg = nullptr;

            int rc = sqlite3_exec(db, createEmployeeKeyResultsTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 



    cout<<"Welcome to OK Results."<<endl;
    string input = "";

//Exit program

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

//Add to table
        else if(input == "create" || input == "new"){ 
            cout<<"What type of object would you like to create?"<<endl; 
            cout<<"OKR Object types are: team, employee, key result, employee key result, and objective"<<endl; 
            input = getInput(); 
            string sql; 
            
            if (input == "employee"){ 
                cout<<"For all fields, don't include any spaces in your input."<<endl;
                string firstName = ""; 
                string lastName = ""; 
                string email = ""; 
                string role = ""; 
                int teamID = -1; 
                cout << "First Name: "; 
                cin >> firstName; cout << "Last Name: "; 
                cin >> lastName; cout << "Email: "; 
                cin >> email; cout << "Role: "; 
                cin >> role; cout << "Team ID: "; 
                cin >> teamID; 
                sql = "INSERT INTO employee " "(FirstName, LastName, Email, Role, TeamID) VALUES (' " + 
                firstName + "', '" + lastName + "', '" + email + "', '" + role + "', " + to_string(teamID) + ");"; 
                
                
                

            
            }else if (input == "team"){ 
                string teamName = ""; 
                cout << "Enter Team Name: "; 
                getline(cin, teamName); 
                sql = "INSERT INTO team (TeamName) VALUES ('" + teamName + "');";

            }else if (input == "objective" || input == "objectives") {

            string title;
            cout << "Title: ";
            cin >> title;

            string desc;
            cout << "Description: ";
            cin >> desc;

            string startDate;
            cout << "Start Date: ";
            cin >> startDate;

            string endDate;
            cout << "End Date: ";
            cin >> endDate;

            string status;
            cout << "Status: ";
            cin >> status;

            int teamID;
            cout << "Team ID: ";
            cin >> teamID;

            int ownerID;
            cout << "Owner Employee ID (or 0 if none): ";
            cin >> ownerID;

            sql = "INSERT INTO objectives "
                "(Title, Description, StartDate, EndDate, Status, TeamID, OwnerEmployeeID) "
                "VALUES ('" + title + "', '" + desc + "', '" + startDate + "', '" +
                endDate + "', '" + status + "', " +
                to_string(teamID) + ", " + (ownerID == 0 ? "NULL" : to_string(ownerID)) + ");";

}else if (input == "team"){
        
            string teamName = "";
            cout << "Enter Team Name: ";
            cin.ignore();
            getline(cin, teamName);

            sql = "INSERT INTO team (TeamName) VALUES ('" + teamName + "');";
        

            }else if (input == "objective" || input == "objectives"){
                string title;
                cout<<"Please provide a title:"<<endl;
                cin>>title;

                string desc;
                cout<<"Please provide a description:"<<endl;
                cin>>desc;

                string startDate;
                cout<<"Please provide the start date:"<<endl;
                cin>>startDate;

                string endDate;
                cout<<"Please provide the end date:"<<endl;
                cin>>endDate;

                string status;
                cout<<"what is the current status of the objective?"<<endl;
                cin>>status;

                sql = "INSERT INTO objectives (Title, Description, StartDate, EndDate, Status) VALUES ('" + title + "', '"+ desc + "', '"+ startDate +"', '" 
                + endDate + "', '"+ status + "');";


            }else if (input == "key_result"){
                //create objective
            }else if (input == "employee_key_result"){
                //create ekr
            }else{
                cout<<"inproper object type."<<endl;
            }

            char* errMsg = nullptr;

            sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

            if (errMsg) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            }
            sql.clear();
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
        
        }else if (input == "join" || input == "combine") {
            //add this

        }else if (input == "quit" || input == "q"){
            cout<<"quitting..."<<endl;
            break;
        }else{
            cout<<"Unidentified input. type help for a list of commands."<<endl;
        }

        

    }

sqlite3_close(db);

}


string printDate(int myDate){
    
    return "printDate is currently not programmed";


}



