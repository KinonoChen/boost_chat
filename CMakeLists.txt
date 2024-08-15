cmake_minimum_required(VERSION 3.10)
project(ChatProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置Boost库和MySQL Connector/C++库的路径
set(BOOST_ROOT "D:/work_study/boost_1_85_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS system)


#set(MySQLConnectorCpp_ROOT "D:/work_study/mysql-connector-c++-8.0.32-winx64")
#set(MySQLConnectorCpp_LIBRARIES "${MySQLConnectorCpp_ROOT}/lib64/vs14/mysqlcppconn.lib")
# 设置MySQL Connector/C++库的路径
set(MySQLConnectorCpp_ROOT "D:/work_study/mysql-connector-c++-8.0.32-winx64-debug/mysql-connector-c++-8.0.32-winx64")
include_directories(${MySQLConnectorCpp_ROOT}/include)
include_directories(${MySQLConnectorCpp_ROOT}/include/jdbc)
include_directories(${MySQLConnectorCpp_ROOT}/include/mysql)
include_directories(${MySQLConnectorCpp_ROOT}/include/mysqlx)
set(MySQLConnectorCpp_LIBRARIES "${MySQLConnectorCpp_ROOT}/lib64/vs14/debug/mysqlcppconn.lib")

# 添加MySQL Connector/C++库到链接库中
link_libraries(${MySQLConnectorCpp_LIBRARIES})

# 添加头文件目录
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 链接库目录
link_directories(${BOOST_LIBRARYDIR})

# 设置源文件
set(SERVER_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/server_main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/db/db_mannager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chat_server/server.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threadpool/threadpool.cpp
)
set(CLIENT_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/client_main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chat_client/client.cpp
)

# 创建可执行文件
add_executable(tcpServer ${SERVER_SOURCE})
add_executable(tcpClient ${CLIENT_SOURCE})

# 链接Boost系统库
target_link_libraries(tcpServer Boost::system)
target_link_libraries(tcpClient Boost::system)