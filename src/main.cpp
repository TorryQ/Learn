#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h> 
#include <iomanip>

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

void create_table(Connection *con) {
    try {
        std::unique_ptr<Statement> stmt(con->createStatement());
        stmt->execute("CREATE TABLE IF NOT EXISTS employees (name VARCHAR(255), age INT, id VARCHAR(255), department VARCHAR(255))");
    } catch (SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
}

void insert_data(Connection *con, const Employee &A) {
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

int main() {
    try {
        MySQL_Driver driver;
        std::unique_ptr<Connection> con(driver.connect("tcp://127.0.0.1:3306", "root", "123456"));
        con->setSchema("SCdata_data");

        create_table(con.get());

        Employee A;
        cin >> A.name >> A.age >> A.id >> A.department;
        insert_data(con.get(), A);

    } catch (SQLException &e) {
        cout << "SQL Error: " << e.what() << endl;
    }
    return 0;
}
