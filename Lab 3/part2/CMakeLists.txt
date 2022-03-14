add_executable(lab3p2
        "barrier.c"
        "barrier.h"
        "test_barrier.c")
TARGET_LINK_LIBRARIES(lab3p2
        pthread
        )