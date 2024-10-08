# Copyright 2023 Google LLC. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd

# Copy the pssh-box.py script and its protos into the required directory
# structure.
set(PSSH_BOX_OUTPUTS
    ${CMAKE_BINARY_DIR}/packager/pssh-box.py
    ${CMAKE_BINARY_DIR}/packager/pssh-box-protos
    ${CMAKE_BINARY_DIR}/packager/pssh-box-protos/widevine_common_encryption_pb2.py
    ${CMAKE_BINARY_DIR}/packager/pssh-box-protos/widevine_pssh_data_pb2.py)

add_custom_command(
    DEPENDS
        protobuf_py
        widevine_protos
        pssh-box.py
    OUTPUT ${PSSH_BOX_OUTPUTS}
    COMMAND
        ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/packager/pssh-box-protos/
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/packager/media/base/widevine_common_encryption_pb2.py
        ${CMAKE_BINARY_DIR}/packager/media/base/widevine_pssh_data_pb2.py
        ${CMAKE_BINARY_DIR}/packager/pssh-box-protos/
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/packager/third_party/protobuf/py/google
        ${CMAKE_BINARY_DIR}/packager/pssh-box-protos/google
    COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/pssh-box.py
        ${CMAKE_BINARY_DIR}/packager/)

add_custom_target(pssh_box_py ALL DEPENDS ${PSSH_BOX_OUTPUTS})

# Always install the python tools.
install(PROGRAMS ${CMAKE_BINARY_DIR}/packager/pssh-box.py
        DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/packager/pssh-box-protos
        DESTINATION ${CMAKE_INSTALL_BINDIR})
