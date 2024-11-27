# Version de cmake demandée.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
# Chemin des répertoires contenant les fichiers entêtes.
INCLUDE_DIRECTORIES( src/include )

# Chemin du répertoire contenant les binaires.
SET ( EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# Packages requis.
FIND_PACKAGE( TBB ) 

# Création des exécutables.
ADD_EXECUTABLE( testMain 
                src/Metrics.cpp
		src/testMain.cpp
)

# Librairies avec lesquelles linker.
TARGET_LINK_LIBRARIES( testMain TBB::tbb )

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )