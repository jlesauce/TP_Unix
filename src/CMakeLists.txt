set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SOURCES
        exercise01.c
        exercise02_1.c
        exercise02_2.c
        exercise03.c
        exercise04.c
        exercise05.c
        exercise06.c
        exercise07.c
        exercise08.c
        exercise09.c
        exercise10.c
        exercise11.c
        exercise12.c
        exercise13_1.c
        exercise13_2.c
        exercise15_client.c
        exercise15_server.c)

foreach (SOURCE_FILE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
endforeach ()
