cmake_minimum_required(VERSION 3.11)
project(SCdata_2 VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find MySQL Connector C++ library
set(MySQLConnectorC++_DIR "/usr/local/mysql/connector-c++-9.0.0/lib64/cmake")
find_package(MySQLConnectorC++ REQUIRED)
include_directories(${MySQLConnectorC++_INCLUDE_DIRS})
link_directories(${MySQLConnectorC++_LIBRARY_DIRS})

# Add your executable target
add_executable(SCdata_2 src/main.cpp)

# Link the MySQL Connector C++ library
target_link_libraries(SCdata_2 ${MySQLConnectorC++_LIBRARIES})

# Install rules (optional)
install(TARGETS SCdata_2 DESTINATION bin)
