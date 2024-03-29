file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_DEMO_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_executable(LangulusModFTXUIDemo ${LANGULUS_MOD_FTXUI_DEMO_SOURCES})

target_link_libraries(LangulusModFTXUIDemo
	PRIVATE		Langulus
				$<$<NOT:$<BOOL:${WIN32}>>:pthread>
)

add_dependencies(LangulusModFTXUIDemo
	LangulusModFTXUI
)
