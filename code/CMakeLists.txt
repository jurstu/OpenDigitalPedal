cmake_minimum_required (VERSION 3.16)
project(guitarEffect)

find_library(PA portaudio)

file(GLOB MAIN_OUT_SRC
    "inc/*.h"
    "src/*.cpp"
)

add_executable(effect_out ${MAIN_OUT_SRC})
target_link_libraries(effect_out PUBLIC "${PA}")