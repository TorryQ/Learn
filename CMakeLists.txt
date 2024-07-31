cmake_minimum_required(VERSION 3.11)  # FetchContent is available in 3.11+
project(SCdata_2 VERSION 1.0.0 LANGUAGES CXX)

# Find MySQL Connector C++ library
# find_package(MySQLConnectorC++ REQUIRED)

include_directories("/usr/local/mysql/connector-c++-9.0.0/include/jdbc/cppconn") # I
link_directories("/usr/local/mysql/connector-c++-9.0.0/lib64") # L

# Create an executable target
add_executable(SCdata_2 src/main.cpp)

# Link the MySQL Connector C++ library
target_link_libraries(SCdata_2 mysqlcppconn) # l

# Specify the C++ standard
set_target_properties(SCdata_2 PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED True
)

# Install rules
install(TARGETS SCdata_2 DESTINATION bin)
