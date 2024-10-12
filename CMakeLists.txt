cmake_minimum_required(VERSION 3.28)
project(LangulusModFTXUI
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for handling user interface using FTXUI library"
    HOMEPAGE_URL    https://langulus.com
)

# Configure FTXUI library, it will be statically built inside this module		
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

fetch_external_module(
    FTXUI
    GIT_REPOSITORY  https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG         1d40687a403b7babdafdd577d10e001840b0f124
)

file(GLOB_RECURSE
    LANGULUS_MOD_FTXUI_SOURCES 
    LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
    source/*.cpp
)

# Build the module                                                              
add_library(LangulusModFTXUI SHARED ${LANGULUS_MOD_FTXUI_SOURCES})

target_link_libraries(LangulusModFTXUI 
    PRIVATE     Langulus
                ftxui::screen
                ftxui::dom
                ftxui::component
)

target_include_directories(LangulusModFTXUI
    PRIVATE     ${FTXUI_SOURCE_DIR}
)

if(LANGULUS_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

#add_subdirectory(demo)
