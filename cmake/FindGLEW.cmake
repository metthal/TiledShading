SET(GLEW_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${GLEW_PATH}
)

FIND_PATH(GLEW_INCLUDE_DIR
    NAMES GL/glew.h
	PATH_SUFFIXES include/GLEW include
	PATHS ${GLEW_SEARCH_PATHS}
)

FIND_LIBRARY(GLEW_LIBRARY
	NAMES glew32.lib
	PATH_SUFFIXES lib64 lib lib/x64 lib/x86
	PATHS ${GLEW_SEARCH_PATHS}
)

FIND_FILE(GLEW_RUNTIME_LIBRARY
    NAMES glew32.dll
	PATH_SUFFIXES bin
	PATHS ${GLEW_SEARCH_PATHS}
)