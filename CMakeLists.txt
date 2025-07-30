cmake_minimum_required(VERSION 3.16)
project(MySFMLGame)

#enable_language(OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -O3")
if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Спроба знайти rc.exe вручну
    if(EXISTS "C:/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/x64/rc.exe")
        set(CMAKE_RC_COMPILER "C:/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/x64/rc.exe" CACHE FILEPATH "Path to RC compiler" FORCE)
    else()
        message(WARNING "rc.exe not found, disabling resource compiler")
        set(CMAKE_RC_COMPILER_WORKS TRUE)
    endif()
endif()

# SFML
if(SFML_FOUND)
message(STATUS "Using locale SFML")
else()
    include(FetchContent)
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
    )
    FetchContent_MakeAvailable(sfml)
endif()


# Sources
set(SOURCES
${CMAKE_SOURCE_DIR}/src/main.cpp
${CMAKE_SOURCE_DIR}/src/Player.cpp
${CMAKE_SOURCE_DIR}/src/Entity.cpp
${CMAKE_SOURCE_DIR}/src/view.cpp
${CMAKE_SOURCE_DIR}/src/Enemy.cpp
${CMAKE_SOURCE_DIR}/src/key.cpp
${CMAKE_SOURCE_DIR}/src/TmxLevel.cpp
${CMAKE_SOURCE_DIR}/src/menu.cpp
${CMAKE_SOURCE_DIR}/src/fire.cpp
${CMAKE_SOURCE_DIR}/src/trap.cpp
${CMAKE_SOURCE_DIR}/src/items.cpp
${CMAKE_SOURCE_DIR}/src/startGame.cpp
${CMAKE_SOURCE_DIR}/src/healthBar.cpp
${CMAKE_SOURCE_DIR}/tinyxml2/tinyxml2.cpp
)

if(APPLE)
    add_executable(game MACOSX_BUNDLE ${SOURCES})

    set(APP_DIR "${CMAKE_BINARY_DIR}/Orc_gameDir")

    set(DMG_FILE "${CMAKE_BINARY_DIR}/Orc_game.dmg")

    add_custom_command(
    OUTPUT ${APP_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/game.app" ${APP_DIR}/game.app
    COMMENT "Preparing app directory for DMG"
    )

    add_custom_command(
    OUTPUT ${DMG_FILE}
    DEPENDS ${APP_DIR} game
    COMMAND hdiutil create -volname "Orc_game" -srcfolder ${APP_DIR} -ov -format UDZO ${DMG_FILE}
    COMMENT "Creating DMG file"
    )

    set_target_properties( game PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "Orc_Game"
    MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/AppIcon.icns"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )

    file(GLOB_RECURSE IMAGE_FILES
    ${CMAKE_SOURCE_DIR}/image/*)

    file(GLOB_RECURSE MUSIC_FILES
    ${CMAKE_SOURCE_DIR}/music/*)

    file(GLOB_RECURSE FONT_FILES
    ${CMAKE_SOURCE_DIR}/font/*)

    file(GLOB_RECURSE STATISTICS_FILES
    ${CMAKE_SOURCE_DIR}/statistics/*)

    set(RESOURCE_FILES ${IMAGE_FILES} ${MUSIC_FILES} ${FONT_FILES} ${STATISTICS_FILES})

    
    set_source_files_properties(${IMAGE_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/image")
    
    set_source_files_properties(${MUSIC_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/music")
    
    set_source_files_properties(${FONT_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/font")
    
    set_source_files_properties(${STATISTICS_FILES}
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/statistics")

    set_source_files_properties(AppIcon.icns PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources")

    target_sources(game PRIVATE ${RESOURCE_FILES})
    target_sources(game PRIVATE AppIcon.icns)

    install(TARGETS game BUNDLE DESTINATION .)

    add_custom_target(make_dmg ALL DEPENDS ${DMG_FILE})
else()
    add_executable(game ${SOURCES})
endif()

target_include_directories(game PRIVATE
    ${sfml_SOURCE_DIR}/include
    ${tinyxml2_SOURCE_DIR}
)

# ВАЖЛИВО: вручну додати бібліотеки
target_link_libraries(game
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    "-framework Cocoa"
)

if(SFML_FOUND)
    target_include_directories(game PRIVATE ${SFML_INCLUDE_DIRS})
endif()

