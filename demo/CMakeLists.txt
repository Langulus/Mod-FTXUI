file(GLOB_RECURSE
	LANGULUS_MOD_FTXUI_DEMO_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_langulus_app(LangulusModFTXUIDemo
	SOURCES			${LANGULUS_MOD_FTXUI_DEMO_SOURCES}
	LIBRARIES		Langulus
					$<$<NOT:$<BOOL:${WIN32}>>:pthread>
	DEPENDENCIES    LangulusModFTXUI
)