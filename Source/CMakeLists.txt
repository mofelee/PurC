# -----------------------------------------------------------------------------
# Add components directories
# -----------------------------------------------------------------------------
#if (NOT USE_SYSTEM_MALLOC)
#    add_subdirectory(bmalloc)
#endif ()

add_subdirectory(WTF)

add_subdirectory(PurC)

add_subdirectory(ExtDVObjs)

add_subdirectory(Samples)

add_subdirectory(Tools)

if (ENABLE_API_TESTS)
    add_subdirectory(ThirdParty/gtest)
    add_subdirectory(valgrind)
    add_subdirectory(test)
endif ()

PURC_INCLUDE_CONFIG_FILES_IF_EXISTS()
