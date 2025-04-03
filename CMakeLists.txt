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
    GIT_REPOSITORY  https://github.com/Epixu/FTXUI.git
    GIT_TAG         main
)

file(GLOB_RECURSE
    LANGULUS_MOD_FTXUI_SOURCES 
    LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
    source/*.cpp
)

# Build the module                                                              
add_langulus_mod(LangulusModFTXUI
	${LANGULUS_MOD_FTXUI_SOURCES}
	$<TARGET_OBJECTS:ftxui::screen>
	$<TARGET_OBJECTS:ftxui::dom>
	$<TARGET_OBJECTS:ftxui::component>
)

target_include_directories(LangulusModFTXUI
    PRIVATE     ${FTXUI_SOURCE_DIR}/include
)

if(LANGULUS_TESTING)
	enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(demo)
