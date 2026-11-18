add_executable(unittests unittests.cpp)
target_link_libraries(unittests bls12-381)

add_test(NAME bls12-381 COMMAND unittests)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  add_test(NAME bls12-381-nobmi2 COMMAND unittests)
  set_tests_properties(bls12-381-nobmi2 PROPERTIES ENVIRONMENT "BLS_DISABLE_BMI2=1")
endif()

# Compares the C++ arithmetic with the x86_64 assembly. The C++ arithmetic is built a second time, under another
# namespace, so that both link into one executable.
if(BLS12_381_X86_64_ASM)
  add_library(bls12-381-portable STATIC ${BLS12-381_SOURCES} portable_arithmetic.cpp)
  target_include_directories(bls12-381-portable PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_compile_features(bls12-381-portable PUBLIC cxx_std_20)
  target_compile_definitions(bls12-381-portable PRIVATE bls12_381=bls12_381_portable BLS12_381_PORTABLE_ARITHMETIC)

  add_executable(arithmetic_differential arithmetic_differential.cpp)
  target_link_libraries(arithmetic_differential bls12-381 bls12-381-portable)

  add_test(NAME bls12-381-arithmetic-differential COMMAND arithmetic_differential)
  add_test(NAME bls12-381-arithmetic-differential-nobmi2 COMMAND arithmetic_differential)
  set_tests_properties(bls12-381-arithmetic-differential-nobmi2 PROPERTIES ENVIRONMENT "BLS_DISABLE_BMI2=1")
endif()
