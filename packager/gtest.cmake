# Copyright 2022 Google LLC. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd

# Define a custom function to create protobuf libraries.  This is similar to
# the one defined in the CMake FindProtobuf.cmake module, but allows us to more
# easily hook into the protobuf submodule to do the work instead of searching
# for a system-wide installation.

function(add_gtest NAME)
  cmake_parse_arguments(PARSE_ARGV
      # How many arguments to skip (1 for the library name)
      1
      # Prefix for automatic variables created by the parser
      ADD_GTEST
      # Boolean flags
      ""
      # One-value arguments
      "WORKING_DIRECTORY"
      # Multi-value arguments
      "")

  if(${ADD_GTEST_WORKING_DIRECTORY})
    set(ADD_GTEST_WORKING_DIRECTORY_SETTING
        WORKING_DIRECTORY ${ADD_GTEST_WORKING_DIRECTORY})
  else()
    set(ADD_GTEST_WORKING_DIRECTORY_SETTING)
  endif()

  # Where we output test results in junit format.
  set(ADD_GTEST_REPORT_PATH
      ${PROJECT_SOURCE_DIR}/junit-reports/TEST-${NAME}.xml)

  # Output gtest results in junit format in a consistent place.
  add_test(NAME ${NAME}
           COMMAND ${NAME} --gtest_output=xml:${ADD_GTEST_REPORT_PATH}
           ${ADD_GTEST_WORKING_DIRECTORY_SETTINGS})
endfunction()
