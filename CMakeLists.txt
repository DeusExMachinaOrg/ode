cmake_minimum_required(VERSION 3.10)
project(ODE VERSION 0.8.0 LANGUAGES C CXX)

# Options (can be set from parent CMakeLists.txt)
option(ODE_DOUBLE_PRECISION "Use double precision math" OFF)
option(ODE_WITH_TRIMESH "Enable trimesh support" ON)
option(ODE_WITH_OPCODE "Use OPCODE for trimesh" ON)

# Configuration
set(ODE_PRECISION "dSINGLE")
if(ODE_DOUBLE_PRECISION)
    set(ODE_PRECISION "dDOUBLE")
endif()

# Create config.h
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build/config-default.h)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/build/config-default.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ode/config.h
        COPYONLY
    )
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/include/ode/config.h CONFIG_CONTENT)
    if(ODE_DOUBLE_PRECISION)
        string(REGEX REPLACE "#define dSINGLE 1" "#define dDOUBLE 1" CONFIG_CONTENT "${CONFIG_CONTENT}")
    else()
        string(REGEX REPLACE "#define dDOUBLE 1" "#define dSINGLE 1" CONFIG_CONTENT "${CONFIG_CONTENT}")
    endif()
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/include/ode/config.h "${CONFIG_CONTENT}")
endif()

# ODE Core sources (verified for ODE 0.8)
set(ODE_CORE_SOURCES
    ode/src/array.cpp
    ode/src/box.cpp
    ode/src/capsule.cpp
    ode/src/collision_cylinder_box.cpp
    ode/src/collision_cylinder_plane.cpp
    ode/src/collision_cylinder_sphere.cpp
    ode/src/collision_kernel.cpp
    ode/src/collision_quadtreespace.cpp
    ode/src/collision_space.cpp
    ode/src/collision_transform.cpp
    ode/src/collision_util.cpp
    ode/src/convex.cpp
    ode/src/cylinder.cpp
    ode/src/error.cpp
    ode/src/fastdot.c
    ode/src/fastldlt.c
    ode/src/fastlsolve.c
    ode/src/fastltsolve.c
    ode/src/joint.cpp
    ode/src/lcp.cpp
    ode/src/mass.cpp
    ode/src/mat.cpp
    ode/src/matrix.cpp
    ode/src/memory.cpp
    ode/src/misc.cpp
    ode/src/obstack.cpp
    ode/src/ode.cpp
    ode/src/odemath.cpp
    ode/src/plane.cpp
    ode/src/quickstep.cpp
    ode/src/ray.cpp
    ode/src/rotation.cpp
    ode/src/sphere.cpp
    ode/src/step.cpp
    ode/src/stepfast.cpp
    ode/src/timer.cpp
    ode/src/util.cpp
)

# Joint sources
set(ODE_JOINT_SOURCES
    ode/src/joints/amotor.cpp
    ode/src/joints/ball.cpp
    ode/src/joints/contact.cpp
    ode/src/joints/fixed.cpp
    ode/src/joints/hinge.cpp
    ode/src/joints/hinge2.cpp
    ode/src/joints/joint.cpp
    ode/src/joints/null.cpp
    ode/src/joints/pr.cpp
    ode/src/joints/slider.cpp
    ode/src/joints/universal.cpp
)

# Trimesh sources (OPCODE)
set(ODE_TRIMESH_SOURCES)
set(OPCODE_SOURCES)
set(ICE_SOURCES)

if(ODE_WITH_TRIMESH AND ODE_WITH_OPCODE)
    set(ODE_TRIMESH_SOURCES
        ode/src/collision_trimesh_box.cpp
        ode/src/collision_trimesh_ccylinder.cpp
        ode/src/collision_trimesh_distance.cpp
        ode/src/collision_trimesh_opcode.cpp
        ode/src/collision_trimesh_plane.cpp
        ode/src/collision_trimesh_ray.cpp
        ode/src/collision_trimesh_sphere.cpp
        ode/src/collision_trimesh_trimesh.cpp
    )
    
    set(OPCODE_SOURCES
        OPCODE/OPC_AABBCollider.cpp
        OPCODE/OPC_AABBTree.cpp
        OPCODE/OPC_BaseModel.cpp
        OPCODE/OPC_Collider.cpp
        OPCODE/OPC_Common.cpp
        OPCODE/OPC_HybridModel.cpp
        OPCODE/OPC_LSSCollider.cpp
        OPCODE/OPC_MeshInterface.cpp
        OPCODE/OPC_Model.cpp
        OPCODE/OPC_OBBCollider.cpp
        OPCODE/OPC_OptimizedTree.cpp
        OPCODE/OPC_Picking.cpp
        OPCODE/OPC_PlanesCollider.cpp
        OPCODE/OPC_RayCollider.cpp
        OPCODE/OPC_SphereCollider.cpp
        OPCODE/OPC_TreeBuilders.cpp
        OPCODE/OPC_TreeCollider.cpp
        OPCODE/OPC_VolumeCollider.cpp
    )
    
    set(ICE_SOURCES
        OPCODE/Ice/IceAABB.cpp
        OPCODE/Ice/IceContainer.cpp
        OPCODE/Ice/IceHPoint.cpp
        OPCODE/Ice/IceIndexedTriangle.cpp
        OPCODE/Ice/IceMatrix3x3.cpp
        OPCODE/Ice/IceMatrix4x4.cpp
        OPCODE/Ice/IceOBB.cpp
        OPCODE/Ice/IcePlane.cpp
        OPCODE/Ice/IcePoint.cpp
        OPCODE/Ice/IceRandom.cpp
        OPCODE/Ice/IceRay.cpp
        OPCODE/Ice/IceRevisitedRadix.cpp
        OPCODE/Ice/IceSegment.cpp
        OPCODE/Ice/IceTriangle.cpp
        OPCODE/Ice/IceUtils.cpp
    )
endif()

# Create static library
add_library(ode STATIC
    ${ODE_CORE_SOURCES}
    ${ODE_JOINT_SOURCES}
    ${ODE_TRIMESH_SOURCES}
    ${OPCODE_SOURCES}
    ${ICE_SOURCES}
)

# Include directories
target_include_directories(ode
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ode/src
)

if(ODE_WITH_TRIMESH AND ODE_WITH_OPCODE)
    target_include_directories(ode PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/OPCODE
        ${CMAKE_CURRENT_SOURCE_DIR}/OPCODE/Ice
    )
    target_compile_definitions(ode PUBLIC 
        dTRIMESH_ENABLED 
        dTRIMESH_OPCODE
    )
endif()

# Compiler definitions
target_compile_definitions(ode PUBLIC ${ODE_PRECISION})
target_compile_definitions(ode PUBLIC ODE_LIB)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(ode PRIVATE 
        WIN32 
        _WINDOWS 
        _CRT_SECURE_NO_WARNINGS
    )
    if(MSVC)
        target_compile_options(ode PRIVATE 
            /MP                 # Multi-processor compilation
            /W3                 # Warning level 3
            /wd4244             # Disable conversion warnings
            /wd4305             # Disable truncation warnings
            /wd4996             # Disable deprecated warnings
            /EHsc               # Exception handling
        )
    endif()
elseif(UNIX)
    target_compile_options(ode PRIVATE 
        -Wall 
        -Wno-unused-variable
    )
    if(NOT APPLE)
        target_link_libraries(ode PUBLIC m pthread)
    endif()
    set_target_properties(ode PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# C/C++ standards
set_target_properties(ode PROPERTIES
    CXX_STANDARD 98
    CXX_STANDARD_REQUIRED ON
    C_STANDARD 99
)

# Print configuration
message(STATUS "ODE 0.8 Configuration:")
message(STATUS "  Precision:      ${ODE_PRECISION}")
message(STATUS "  Trimesh:        ${ODE_WITH_TRIMESH}")
if(ODE_WITH_TRIMESH)
message(STATUS "  OPCODE:         ${ODE_WITH_OPCODE}")
endif()