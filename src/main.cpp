#include <iostream>
#include <string>
#include <iomanip>
#include <memory>
#include <map>
#include <sstream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using sql::mysql::MySQL_Driver;
using sql::Connection;
using sql::Statement;
using sql::PreparedStatement;
using sql::SQLException;

struct Employee {
    int age;
    string name, id, department;
};

void open()
{
    cout << "SCdata command line tool is a simulative company data management system, which is used to modify the data of the simulative company" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Usage:" << endl << "SCdata [command]" << endl;
    cout << "Available Commands:" << endl;
    cout << "\tAddEmployee\tAddEmployee [ID name age department]" << endl;
    cout << "\tDelEmployee\tDelEmployee [ID]" << endl;
    cout << "\tLookEmployee\tLookEmployee [ID]" << endl;
    cout << "\tModiEmployee\tModiEmployee [ID]" << endl;
    cout << "\tChecktheid\tChecktheid [name]" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    return ;
}

void create_table(Connection *con) 
{
    try {
        std::unique_ptr<Statement> stmt(con->createStatement());
        stmt->execute("CREATE TABLE IF NOT EXISTS employees (name VARCHAR(255), age INT, id VARCHAR(255), department VARCHAR(255))");
    } catch (SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
}

void Add_Employee(Connection *con, const Employee &A) 
{
    try {
        std::unique_ptr<PreparedStatement> pstmt(con->prepareStatement("INSERT INTO employees (name, age, id, department) VALUES(?,?,?,?)"));
        pstmt->setString(1, A.name);
        pstmt->setInt(2, A.age);
        pstmt->setString(3, A.id);
        pstmt->setString(4, A.department);
        pstmt->executeUpdate();
    } catch (SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
}

void Del_Employee(Connection *con, const string del_id)
{
    try {
        std::unique_ptr<PreparedStatement> pstmt(con->prepareStatement("DELETE FROM employees WHERE id = ?"));
        pstmt->setString(1,del_id);
        pstmt->executeUpdate();
    } catch (SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
}

void Look_Employee(Connection *con, const string look_id)
{
  try {
    std::unique_ptr<PreparedStatement> pstmt(con->prepareStatement("SELECT id, name, age, department FROM employees WHERE id = ?"));
    pstmt->setString(1, look_id);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
    while(res->next())
    {
      string Id = res->getString("id");
      string Name = res->getString("name");
      int Age = res->getInt("age");
      string Department = res->getString("department");
      cout << "Find this employee" << endl << "id: " << Id << endl << "name: " << Name << endl << "age: " << Age << endl << " department: " << Department << endl;
    }
  } catch (SQLException &e) {
    cout << "SQL Error: " << e.what() << endl;
  }
}

void Modi_Employee(Connection *con, const string modi_id)
{
  try {
    string opt1, opt2;
    cout << "Which data do you want to change?" << endl;
    cin >> opt1;
    cout << "You want to change it to " << endl;
    cin >> opt2;
    string query = "UPDATE employees SET " + opt1 + " = ? WHERE id = ?";
    std::unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));
    pstmt->setString(1, opt2);
    pstmt->setString(2, modi_id);
    pstmt->executeUpdate();
  } catch (SQLException &e) {
    cout << "SQL Error: " << e.what() << endl;
  }
}

void Check_the_id(Connection *con, const string checkname)
{
  try {
    std::unique_ptr<PreparedStatement> pstmt(con->prepareStatement("SELECT id, department FROM employees WHERE name = ?"));
    pstmt->setString(1, checkname);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
    while(res->next())
    {
      string Id = res->getString("id");
      string Department = res->getString("department");
      cout << "id: " << Id << " department: " << Department << endl;
    }
  } catch (SQLException &e) {
    cout << "SQL Error: " << e.what() << endl;
  }
}

int main() {
    try {
        MySQL_Driver driver;
        std::unique_ptr<Connection> con(driver.connect("tcp://127.0.0.1:3306", "root", "123456"));
        con->setSchema("SCdata_data");
        cout << "Creating table ..." << endl;
        create_table(con.get());
        cout << "Table Created" << endl << endl;
        open();
        string input;
        std::getline(cin, input);
        std::istringstream iss(input);
        string st;
        iss >> st;
        if (st == "AddEmployee")
        {
            Employee A;
            iss >> A.id >> A.name >> A.age >> A.department; 
            Add_Employee(con.get(), A); 
        }else if(st == "DelEmployee")
        {
            string strI;
            iss >> strI;
            Del_Employee(con.get(), strI); 
        }else if(st == "LookEmployee")
        {
            string strI;
            iss >> strI;
            //json res = Look_Employee(strI);
            Look_Employee(con.get(), strI);
        }else if(st == "ModiEmployee")
        {
            string strI;
            iss >> strI;
            Modi_Employee(con.get(), strI);
        }else if(st == "Checktheid")
        {
            string strN;
            iss >> strN;
            Check_the_id(con.get(), strN);
        }else
        {
            cout << "Unknown command, please try again";
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
    return 0;
}
