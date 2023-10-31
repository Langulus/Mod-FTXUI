file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModFTXUITest ${LANGULUS_MOD_FTXUI_TEST_SOURCES})

target_link_libraries(LangulusModFTXUITest
	PRIVATE		Langulus
				Catch2
)

add_test(
	NAME		LangulusModFTXUITest
	COMMAND		LangulusModFTXUITest
)

langulus_copy_dlls(LangulusModFTXUITest ON LangulusModFTXUITest)
langulus_copy_dlls_advanced(LangulusModFTXUITest
	TO			LangulusModFTXUITest
	FROM		LangulusModFTXUI
)