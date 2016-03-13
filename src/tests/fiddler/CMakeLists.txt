project(microf-fiddler)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
)
set(${PROJECT_NAME}_SRCS
    main.cpp
    authhelper.cpp
    res.qrc
    main.qml
    SessionTab.qml
)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SRCS}
)
qt5_use_modules(${PROJECT_NAME} Quick)
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Quick
    microf
)
