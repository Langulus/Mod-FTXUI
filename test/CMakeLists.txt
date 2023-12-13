file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModFTXUITest ${LANGULUS_MOD_FTXUI_TEST_SOURCES})

target_link_libraries(LangulusModFTXUITest
	PRIVATE		Langulus
				Catch2
				$<$<NOT:$<BOOL:${WIN32}>>:pthread>
)

add_dependencies(LangulusModFTXUITest
	LangulusModFTXUI
)

add_test(
	NAME		LangulusModFTXUITest
	COMMAND		LangulusModFTXUITest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)