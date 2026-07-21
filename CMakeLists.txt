### 3.24 because good package finding.
cmake_minimum_required(VERSION 3.24)

### Project declaration.
project(filename-randomizer
  VERSION 0.0    # Version of the program.
  LANGUAGES CXX  # Programming language.
)

### Create the compile_commands.json (mostly for clangd).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### Fix for clangd working on nix.
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
  ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

### Create the executable of the program.
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/utils/utils.cpp
  src/hash/sha256.cpp
  src/argparse/argparse.cpp
)

### Compilation options.
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall   # More errors.
  -Wextra # Even more errors.
)

### Use the c++23 standard.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

### Allow the usage of relative paths for includes of the project.
target_include_directories(${PROJECT_NAME} PRIVATE src)

### Add macro definitions to the program.
target_compile_definitions(${PROJECT_NAME} PRIVATE
  PROJECT_VERSION="${PROJECT_VERSION}" # Version of the program.
)
