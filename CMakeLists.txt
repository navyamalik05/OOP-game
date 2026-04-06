cmake_minimum_required(VERSION 3.30.5)
project(project1)

set(CMAKE_CXX_STANDARD 17)

# --- 1. Find wxWidgets ---
# This tells CMake to use the wx-config script provided by Homebrew
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)
include(${wxWidgets_USE_FILE})

#using conditionals to remedy path errors on other OS than windows
if(WIN32)
    # If we are on Windows, add the WIN32 flag
    add_executable(project1 WIN32
            GameView.cpp
            Game.cpp
            ProjectApp.cpp
            MainFrame.cpp
            Football.cpp
            Item.cpp
            Scoreboard.cpp
            Level.cpp
            Platform.cpp
            ItemVisitor.cpp
            UpdateItemVisitor.cpp
            Background.cpp
            Coin.cpp
            Goalpost.cpp
            CollisionVisitor.cpp
            CollisionVisitor.h
            Wall.cpp
            Wall.h
            Enemy.cpp
            Enemy.h
            Powerup.cpp
            Powerup.h
            DoubleCoinsVisitor.cpp
            DoubleCoinsVisitor.h
            ImageCache.cpp
            ImageCache.h
            BossGoalPost.cpp
            BossGoalPost.h
    )
else()
    # Otherwise (on Mac, Linux, etc.), add without WIN32
    add_executable(project1
            main.cpp
            GameView.cpp
            Game.cpp
            ProjectApp.cpp
            MainFrame.cpp
            Football.cpp
            Item.cpp
            Scoreboard.cpp
            Level.cpp
            Platform.cpp
            ItemVisitor.cpp
            UpdateItemVisitor.cpp
            Background.cpp
            Coin.cpp
            Goalpost.cpp
            CollisionVisitor.cpp
            CollisionVisitor.h
            Wall.cpp
            Wall.h
            Enemy.cpp
            Enemy.h
            Powerup.cpp
            Powerup.h
            DoubleCoinsVisitor.cpp
            DoubleCoinsVisitor.h
            ImageCache.cpp
            ImageCache.h
            BossGoalPost.cpp
            BossGoalPost.h
    )
endif()
# --- 3. Link all libraries to the executable ---
# Now that 'project1' exists, we can link libraries to it.

# Link wxWidgets libraries (using KEYWORD signature)
# THIS IS THE LINE I FIXED
target_link_libraries(project1 PRIVATE ${wxWidgets_LIBRARIES})

# Add necessary Mac frameworks (also using KEYWORD signature)
if(APPLE)
    target_link_libraries(project1 PRIVATE
            "-framework IOKit"
            "-framework Carbon"
            "-framework Cocoa"
            "-framework AudioToolbox"
            "-framework System"
            "-framework OpenGL"
    )
endif()


# --- 4. Copy resource files ---
# Copy the images folder to the build directory after build
add_custom_command(TARGET project1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/levels $<TARGET_FILE_DIR:project1>/levels
)

add_custom_command(TARGET project1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:project1>/images
)


# ===================================================================
# Testing Configuration
# (This part was correct)
# ===================================================================
enable_testing()
include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)


add_executable(runTests
        Tests/LevelTest.cpp
        Level.cpp
        Item.cpp
        Platform.cpp
        Background.cpp
        UpdateItemVisitor.cpp
        Football.cpp
)
target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(runTests PRIVATE gtest gtest_main ${wxWidgets_LIBRARIES})
gtest_discover_tests(runTests)

# Copy resources for the test program
add_custom_command(TARGET runTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/levels $<TARGET_FILE_DIR:runTests>/levels
)
add_custom_command(TARGET runTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:runTests>/images
)