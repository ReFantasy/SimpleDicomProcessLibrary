cmake_minimum_required(VERSION 3.5)

project(SDPL LANGUAGES CXX)

# 库文件输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# 可执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 第三方库
IF (WIN32)
        MESSAGE(STATUS "Now is windows")
		set(Qt5_DIR "D:/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5")
        # DCMTK
        include_directories("E:/dcmtk/include")
        LINK_DIRECTORIES("E:/dcmtk/lib")
        link_libraries(dcmtk.lib)

ELSEIF (APPLE)
        MESSAGE(STATUS "Now is Apple systens.")
        # DCMTK
        include_directories("/usr/local/Cellar/dcmtk/3.6.6/include")
        LINK_DIRECTORIES("/usr/local/Cellar/dcmtk/3.6.6/lib")
        link_libraries(dcmdata dcmimgle oflog dcmpstat ofstd iconv dcmjpeg)

ELSEIF (UNIX)
        MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()


find_package(Qt5 COMPONENTS Widgets REQUIRED)

ADD_SUBDIRECTORY(global)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(test)

