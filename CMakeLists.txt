cmake_minimum_required(VERSION 3.24)
project(LangulusModFTXUI
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for handling user interface using FTXUI library"
    HOMEPAGE_URL    https://langulus.com
)

# Configure FTXUI library, it will be statically built inside this module		
set(FTXUI_ENABLE_INSTALL OFF)
fetch_external_module(
    FTXUI
    GIT_REPOSITORY  https://github.com/ArthurSonzogni/ftxui
    GIT_TAG         v5.0.0
)

file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp source/*.hpp
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

add_subdirectory(demo)
