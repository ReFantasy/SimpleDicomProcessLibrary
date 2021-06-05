cmake_minimum_required(VERSION 3.5)

project(SDPL LANGUAGES CXX)

# 库文件输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# 可执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 第三方库
IF (WIN32)
        MESSAGE(STATUS "Now is windows")
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


ADD_SUBDIRECTORY(global)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(test)

