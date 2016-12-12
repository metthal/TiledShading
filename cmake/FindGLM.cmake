SET(GLM_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${GLM_PATH}
)

FIND_PATH(GLM_INCLUDE_DIR
    NAMES glm/glm.hpp
	PATHS ${GLM_SEARCH_PATHS}
)