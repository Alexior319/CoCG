file(COPY
  pddl/
  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/pddl
)

add_subdirectory(unit)
