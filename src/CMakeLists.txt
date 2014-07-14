#TARGET_LINK_LIBRARIES(scalpel libscalpelredis.a)
ADD_SUBDIRECTORY(linux)
ADD_SUBDIRECTORY(nginx)
ADD_SUBDIRECTORY(php)
ADD_SUBDIRECTORY(redis)
ADD_SUBDIRECTORY(utils)

aux_source_directory(./ SRC_LISTS)
ADD_EXECUTABLE(scalpel ${SRC_LISTS})
TARGET_LINK_LIBRARIES(scalpel scalpelutils)
TARGET_LINK_LIBRARIES(scalpel scalpelredis)

#dependencies
ADD_DEPENDENCIES(scalpelredis_static scalpelutils)
ADD_DEPENDENCIES(scalpel scalpelredis_static)

#install
INSTALL(TARGETS scalpel
    RUNTIME DESTINATION bin)
