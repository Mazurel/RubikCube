cmake_minimum_required(VERSION 3.18)
project(POGK_Rubic)

set(CMAKE_CXX_STANDARD 20)

# Import Ogre library:
# specify which version and components you need
set(OGRE_LOKALIZACJA "C:\\Jakieś miejsce")
find_package(OGRE REQUIRED COMPONENTS Bites RTShaderSystem HINTS ${OGRE_LOKALIZACJA})

message(${OGRE_LIBRARY_DIRS})

function(appendTo OUT_F IN_F)
    file(READ ${IN_F} RSC_BUFFER)
    file(APPEND ${OUT_F} "${RSC_BUFFER}\n")
endfunction()

# copy resource.cfg next to our binaries where OGRE looks for it
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg)
appendTo(${CMAKE_CURRENT_BINARY_DIR}/resources.cfg ${OGRE_CONFIG_DIR}/resources.cfg)
appendTo(${CMAKE_CURRENT_BINARY_DIR}/resources.cfg ${PROJECT_SOURCE_DIR}/resources.cfg)
message("Created resources.cfg")
# Copy image
file(COPY ${PROJECT_SOURCE_DIR}/sky.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images)
file(COPY ${PROJECT_SOURCE_DIR}/skybox.material DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
message("Copied data")

add_executable(POGK_Rubic main.cpp box.cpp rubik.cpp rotator.cpp)

target_link_libraries(POGK_Rubic OgreBites OgreRTShaderSystem)

