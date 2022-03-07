# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/canadiantides-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/canadiantides-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/canadiantides-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME CanadianTides_pi)
set(PKG_VERSION  0.1.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME CanadianTides)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME CanadianTides) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Show Canadian tidal heights for LW/HW")
set(PKG_DESCRIPTION [=[
Show Canadian Tides (HW/LW) at over 600 ports.
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/CanadianTides_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/CanadianTides.html)

set(SRC
    src/bbox.cpp
    src/bbox.h		
    src/CanadianTides_pi.h
    src/CanadianTides_pi.cpp
    src/icons.h
    src/icons.cpp
    src/CanadianTidesgui.h
    src/CanadianTidesgui.cpp
    src/CanadianTidesgui_impl.cpp
    src/CanadianTidesgui_impl.h
	src/NavFunc.cpp
	src/NavFunc.h
	src/tidetable.cpp
	src/tidetable.h
	src/gl_private.h
	src/pidc.cpp
	src/pidc.h

)

set(PKG_API_LIB api-16)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  if (PLUGIN_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC PLUGIN_USE_SVG)
  endif ()

  add_definitions(-DocpnUSE_GL)

  if (QT_ANDROID)
    add_definitions(-DUSE_ANDROID_GLES2)
  endif ()
  
endmacro ()
macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

  add_subdirectory("libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)
endmacro ()
