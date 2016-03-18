project(microf)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_WARNINGS_FLAGS} ${CMAKE_CXX_COVERAGE_FLAGS}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(${PROJECT_NAME}_SRCS
    facebook.cpp
    sessioncontroller.cpp
)

set(${PROJECT_NAME}_INTERNALS_SRCS
    internal/emptyresult.cpp
    internal/emptyresultfactory.cpp
    internal/loginrequestfactory.cpp
    internal/loginresultpreprocessfactory.cpp
    internal/logoutrequestfactory.cpp
)

microgen_bean(${PROJECT_NAME}_CORE_SRCS
    core/loginrequest.yaml
    core/logoutrequest.yaml
    core/session.yaml
)
microgen_qtbean(${PROJECT_NAME}_CORE_SRCS
    core/session.yaml
)
microgen_factory(${PROJECT_NAME}_CORE_SRCS
    core/session.yaml
)

add_library(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SRCS}
    ${${PROJECT_NAME}_CORE_SRCS}
    ${${PROJECT_NAME}_INTERNALS_SRCS}
)
qt5_use_modules(${PROJECT_NAME} Core Network)
target_link_libraries(${PROJECT_NAME}
    microcore
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)


