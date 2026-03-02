#include <iostream>
#include <string>
#include <sqlite3.h> //make sure you install sqlite through linux/ubuntu or something else
//or this will not work. also, if any of the code doesn't work for you, let nate know.
//ignore the squiggly red line, it's harmless. just compile using ubuntu.
//IMPORTANT! use ./program to run! using ./main.cpp will have issues.
using namespace std;

//    g++ main.cpp -lsqlite3 -o program
//just leave this here, so i can copy-paste compile quickly


string getInput(){
    string input = "";
    while (input == ""){
        getline(cin, input);
            for (int i = 0; i < input.size(); i++){ //convert string to lowercase
                input.at(i) = tolower(input.at(i)); 
                if (input.at(i) == ' '){
                    input.at(i) = '_';
                }
            }
            if (input == ""){
                cout<<"Empty input recieved. Please re-enter input: "<<endl;
            }
        }
        return input;
}

int getIntInput(){ //maybe make a version of this for floats?
    string input = "";
    string output = "";
    
    while (output == ""){
    getline(cin, input);
        for (int i = 0; i < input.size(); i++){
            if (isdigit(input.at(i))){ // get all numbers from input in order
                output += input.at(i);
            }
        }
        if (output == ""){ //if there were no numbers in the input
            cout<<"input could not be understood. please enter a positive integer. ";
            
        }
    }
        return stoi(output);
}



static int callback(void* unused, int argc, char** argv, char** azColName) { //Chatgpt made this method, it is used once i think
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": "
             << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "---------------------" << endl;
    return 0;
}


int main(){

sqlite3* db;
char* errMsg = nullptr;
int rc = -1; 
int trolling = 0; //this is used to stop an accidental infinite loop later on

if (sqlite3_open("okr.db", &db) != SQLITE_OK) { //error if cnanot open database file
    cerr << "Cannot open database\n";
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

            errMsg = nullptr;

            rc = sqlite3_exec(db, createTeamTable, nullptr, nullptr, &errMsg);

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


            errMsg = nullptr;
            rc = sqlite3_exec(db, createEmployeeTable, nullptr, nullptr, &errMsg);

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

            errMsg = nullptr;
            rc = sqlite3_exec(db, createObjectivesTable, nullptr, nullptr, &errMsg);

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

            errMsg = nullptr;
            rc = sqlite3_exec(db, createKeyResultsTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 

            //create employee key results table
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

            errMsg = nullptr;
            rc = sqlite3_exec(db, createEmployeeKeyResultsTable, nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            } 


//input loop
    cout<<"Welcome to OK Results."<<endl;
    string input = "";

    //Exit program
    while (!(input == "q" || input == "quit")){
        input = getInput();
        
        //help command
        if (input == "help" || input == "help"){
            //help command
            cout<<"Commands:"<<endl;
            cout<<"---------------------------"<<endl;
            cout<<"Create: Add a new team, employee, Objective, Key Result, or employee key result."<<endl;
            cout<<"Delete: remove some or all data from a database table of your choice"<<endl;
            cout<<"List: Read all data from a database table of your choice"<<endl;
            cout<<"Help: Opens help menu"<<endl;
            cout<<"Update: "<<endl;
            cout<<""<<endl;
            cout<<""<<endl;
            
        }else if (input == "quit" || input == "q"){
            cout<<"quitting..."<<endl;
            break;
        

            //Add data to table
        }else if(input == "create" || input == "new" || input == "add"){ 
            cout<<"What type of object would you like to create?"<<endl; 
            cout<<"OKR Table types are: team, employee, key result, employee key result, and objective"<<endl; 
            input = getInput(); 
            string sql; 
            

            //note: all of these don't execute the sql code immediately, they store the code in the sql variable and execute at the end.
            if (input == "employee"){ 
                
                string firstName; 
                string lastName; 
                string email; 
                string role; 
                string hireDate;
                int teamID; 


                cout << "First Name: "<<endl; firstName = getInput(); 
                cout << "Last Name: "<<endl; lastName = getInput(); 
                cout << "Email: "<<endl; email = getInput(); 
                cout << "Role: "<<endl; role = getInput(); 
                cout << "Employee Hire Date: "<<endl; hireDate = getInput(); 
                cout << "Team ID: "<<endl; teamID = getIntInput(); 

                sql = "INSERT INTO employee (FirstName, LastName, Email, Role, HireDate, TeamID) "
                "VALUES ('" + firstName + "', '" + lastName + "', '" + email + "', '" + role + "', '" + hireDate + "', " + to_string(teamID) + ");"; 
                
                //debug code
                //  cout<<"sql command: " + sql<<endl;
                
                

            
            }else if (input == "team"){ 
                string teamName = ""; 
                int teamID; 

                cout << "Team Name: "<<endl; teamName = getInput();
                cout << "Team ID: "<<endl; teamID = getIntInput();

                sql = "INSERT INTO team (TeamName, TeamID) VALUES ('" + teamName + "', " + to_string(teamID) + ");";
                
            }else if (input == "objective" || input == "objectives") {

            string title;
            string desc;
            string startDate;
            string endDate;
            string status;
            int teamID;
            int ownerID;

            cout << "Title: "<<endl; title = getInput();
            cout << "Description: "<<endl; desc = getInput();
            cout << "Start Date: "<<endl; startDate = getInput();
            cout << "End Date: "<<endl; endDate = getInput();
            cout << "Status: "<<endl; status = getInput();
            cout << "Team ID: "<<endl; teamID = getIntInput();
            cout << "Owner Employee ID (or 0 if none): "<<endl; ownerID = getIntInput();

            sql = "INSERT INTO objectives "
                "(Title, Description, StartDate, EndDate, Status, TeamID, OwnerEmployeeID) "
                "VALUES ('" + title + "', '" + desc + "', '" + startDate + "', '" +
                endDate + "', '" + status + "', " +
                to_string(teamID) + ", " + to_string(ownerID) + ");";
        

            }else if (input == "key_result" || input == "key_results" || input == "kr"){
                int keyResultID;
                int objectiveID;
                string title;
                string metricName;
                float startVal;
                float currVal;
                float targetVal;
                float maxVal;
                string unit;
                string startDate;
                string dueDate;
                string status;
                

            cout << "Key result ID: "<<endl; keyResultID = getIntInput();
            cout << "Targeted Objective ID: "<<endl; objectiveID = getIntInput();
            cout << "Title: "<<endl; title = getInput();
            cout << "Metric Name: "<<endl; metricName = getInput();
        cout << "---------------------------------------------------"<<endl;
            cout << "Completion Starting Value (usually 0): "<<endl; startVal = getIntInput();
            cout << "Completion Current Value: "<<endl; currVal = getIntInput();
            cout << "Completion Max Value (usually 100): "<<endl; maxVal = getIntInput();
            cout << "Completion Target Value: "<<endl; targetVal = getIntInput();
        cout << "---------------------------------------------------"<<endl;
            cout << "Unit: "<<endl; unit = getInput();
            cout << "Start Date: "<<endl; startDate = getInput();
            cout << "Due date: "<<endl; dueDate = getInput();
            cout << "Current Status: "<<endl; status = getInput();


            sql = "INSERT INTO key_results "
                "(KeyResultID, ObjectiveID, Title, MetricName, StartValue, CurrentValue, TargetValue, MaxValue, Unit, StartDate, DueDate, Status) "
                "VALUES (" + to_string(keyResultID) + ", " + to_string(objectiveID) + ", '" + title + "', '" + metricName + "', " 
                + to_string(startVal) + ", " + to_string(currVal) + ", " + to_string(targetVal) + ", " + to_string(maxVal) + ", '"
                + unit + "', '" + startDate + "', '" + dueDate + "', '" + status + "');";


            }else if (input == "employee_key_result" || input == "employee_key_results" || input == "ekr"){
                int employeeID;
                int keyResultID;
                string assDate;
                float weight;
                string status;


                cout << "Employee ID: "<<endl; employeeID = getIntInput();
                cout << "Key result ID: "<<endl; keyResultID = getIntInput();
                cout << "Assigned Date: "<<endl; assDate = getInput();
                cout << "Weight: "<<endl; weight = getIntInput();
                cout << "Status: "<<endl; status = getInput();
       
                sql = "INSERT INTO employee_key_result "
                "(EmployeeID, KeyResultID, AssignedDate, Weight, Status) "
                "VALUES (" + to_string(employeeID) + ", " + to_string(keyResultID) + ", '" + assDate + "', '" + to_string(weight) + "', '" 
                + status + "');";

            }else{
                cout<<"improper object type."<<endl;
                sql = ""; //i haven't tested if this line causes issues or not, but it should be fine.
            }


            //execute sql code
            errMsg = nullptr;

            sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

            if (errMsg) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            }else{
                cout<<"Successfully created table entry of type: " + input <<endl;
            }
            sql.clear();
        }

        //Delete data from table

        else if (input == "delete" || input == "kill") {

            cout << "Delete which object?" << endl << "choose from team, employee, objective, or key result" << endl;
            string sql;
             input = getInput();
             int id = -1;

            if (input == "team" || input == "teams") {
                cout << "Enter Team ID: ";
                id = getIntInput();
                sql = "DELETE FROM team WHERE TeamID = " + to_string(id) + ";";
                

            }else if  (input == "employee" || input == "employees"){
                cout << "Enter Employee ID: ";
                id = getIntInput();
                sql = "DELETE FROM employee WHERE EmployeeID = " + to_string(id) + ";";
                

            }else if  (input == "key_result" || input == "key_results" || input == "kr"){
                cout << "Enter key result ID: ";
                id = getIntInput();
                sql = "DELETE FROM key_results WHERE KeyResultID = " + to_string(id) + ";";
                

            }else if  (input == "objective" || input == "objectives"){
                cout << "Enter Objective ID: ";
                id = getIntInput();
                sql = "DELETE FROM objectives WHERE ObjectiveID = " + to_string(id) + ";";
                

            }else if  (input == "employee_key_result" || input == "employee_key_results"){//unfinished, can't figure out
                cout<<"Employee key results can't currently be deleted. We will work on it though!"<<endl;
            }
            
            errMsg = nullptr;
            sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

            if (errMsg) {
                cout << "SQL error: " << errMsg << endl;
                sqlite3_free(errMsg);
            }else{
                cout<<"Successfully deleted entry of type: " + input <<endl;
            }
            sql.clear();
            



        }else if (input == "list" || input == "view") { //view data UNFINISHED

            cout << "What do you want to list? (team, employee, objective)" << endl;
            input = getInput();

            if (input == "team") {
                cout<<"Viewing teams"<<endl;
                const char* sql = "SELECT * FROM team;";
                sqlite3_exec(db, sql, callback, nullptr, nullptr);
            }else if (input == "employee") {
                cout<<"Viewing employees"<<endl;
                const char* sql = "SELECT * FROM employee;";
                sqlite3_exec(db, sql, callback, nullptr, nullptr);
            }else if (input == "objective") {
                cout<<"Viewing objective"<<endl;
                const char* sql = "SELECT * FROM objective;";
                sqlite3_exec(db, sql, callback, nullptr, nullptr);
            }





        }else if (input == "update" || input == "edit") {
            //add this
        
        }else if (input == "join" || input == "combine") {
            //add this (maybe)

        }else{
            cout<<"Unidentified input. type help for a list of commands."<<endl;
            trolling++;
            if (trolling >= 15){
                cout<<"An error has likely occured, or you have entered an incorrect input too many times. The program will now quit" <<endl;
                break;
            }
        }

        

    }

sqlite3_close(db);

}


string printDate(int myDate){
    
    return "printDate is currently not programmed";


}



