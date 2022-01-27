#file(GLOB_RECURSE srcs CONFIGURE_DEPENDS *.c *.h)
#add_executable(lab1p1 ${srcs})

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
add_executable(lab1p1
        "queue.h"
        "queue.c"
        "lab1p1.c")

add_executable(lab1p1e
        "queue.h"
        "queue.c"
        "testr.c")



#set_target_properties(lab1p1 PROPERTIES LINKER_LANGUAGE C)