list(APPEND TriangularMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND TriangularMesh_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND TriangularMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/TriangularMesh.hpp)

list(APPEND TriangularMesh_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(TriangularMesh_sources ${TriangularMesh_sources} PARENT_SCOPE)
set(TriangularMesh_headers ${TriangularMesh_headers} PARENT_SCOPE)
set(TriangularMesh_includes ${TriangularMesh_includes} PARENT_SCOPE)
