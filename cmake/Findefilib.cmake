find_path(EFILIB_INCLUDE_DIR efi.h
          HINTS "/usr/include/efi"
          )

add_library(efilib INTERFACE)
add_library(efilib::efilib ALIAS efilib)

target_include_directories(efilib INTERFACE ${EFILIB_INCLUDE_DIR} "${EFILIB_INCLUDE_DIR}/x86_64")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(efilib DEFAULT_MSG EFILIB_INCLUDE_DIR)

mark_as_advanced(EFILIB_INCLUDE_DIR)