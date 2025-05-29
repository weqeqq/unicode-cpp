@PACKAGE_INIT@

set_and_check(UNICODE_INCLUDE_DIR @PACKAGE_UNICODE_INCLUDE_DIR@)
set_and_check(UNICODE_LIBRARY_DIR @PACKAGE_UNICODE_LIBRARY_DIR@)

include(${CMAKE_CURRENT_LIST_DIR}/unicode-targets.cmake)
check_required_components(unicode)
