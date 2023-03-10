cmake_minimum_required(VERSION 3.5)

project(hello_s2j)

############################################################
# Set compile flags
############################################################
set(CMAKE_C_FLAGS_DEBUG "-std=c89 -Werror -Wall -Wundef -Wno-unused-but-set-variable -Wno-misleading-indentation -Wno-unused-local-typedefs -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-unused-function -Wno-unused-value -Wno-unused-result -Wno-strict-aliasing -Wno-format-truncation -Wno-discarded-qualifiers -Wno-error=implicit-function-declaration -fdata-sections -ffunction-sections -g -D_DEBUG -DDEBUG -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELEASE "-std=c89 -Wall -Wundef -Wno-unused-but-set-variable -Wno-misleading-indentation -Wno-unused-local-typedefs -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-unused-function -Wno-unused-value -Wno-unused-result -Wno-strict-aliasing -Wno-format-truncation -fvisibility=hidden -fdata-sections -ffunction-sections -DNDEBUG -Os")
set(CMAKE_BUILD_TYPE Debug)
############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(struct2json SHARED 
    "${PROJECT_SOURCE_DIR}/cJSON/cJSON.c"
    "${PROJECT_SOURCE_DIR}/source/struct2json.c"
    "${PROJECT_SOURCE_DIR}/source/util_vector.c"
)

include_directories(
    "${PROJECT_SOURCE_DIR}/cJSON"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/demo/example_cases"
)

############################################################
# Create an executable
############################################################

aux_source_directory("${PROJECT_SOURCE_DIR}/demo/example_cases" SOURCE_FILE)
# Add an executable with the above sources
add_executable(hello_s2j
    ${SOURCE_FILE}
    "${PROJECT_SOURCE_DIR}/demo/main.c"
)

# link the new hello_s2j target with the hello_s2j target
target_link_libraries( hello_s2j
    PRIVATE 
        struct2json
)
