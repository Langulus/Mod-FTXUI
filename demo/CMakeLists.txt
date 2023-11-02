file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_DEMO_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModFTXUIDemo ${LANGULUS_MOD_FTXUI_DEMO_SOURCES})

target_link_libraries(LangulusModFTXUIDemo
	PRIVATE		Langulus
)

langulus_copy_dlls(LangulusModFTXUIDemo ON LangulusModFTXUIDemo)
langulus_copy_dlls_advanced(LangulusModFTXUIDemo
	TO			LangulusModFTXUIDemo
	FROM		LangulusModFTXUI
)