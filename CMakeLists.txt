# set project's name
  PROJECT( edbfvg )
  
  cmake_minimum_required(VERSION 2.8)
  
  # with SET() command you can change variables or define new ones
  # here we define EDBFVG_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( EDBFVG_SRCS
    main.cpp
    mainwindow.cpp
	editdialog.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( EDBFVG_MOC_HDRS
    mainwindow.h
	editdialog.h
  )
  
  SET( EDBFVG_UIS
	mainwindow.ui
	editdialog.ui
    )
    
  
    SET( EDBFVG_RCS
        rc.qrc
  )
  
  set(EXECUTABLE_OUTPUT_PATH ${edbfvg_SOURCE_DIR})
  
  # and finally an resource file
  
  # enable warnings
  ADD_DEFINITIONS( -Wall )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  SET( QT_USE_SQL TRUE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )
  
  # this command will generate rules that will run rcc on all files from EDBFVG_RCS
  # in result EDBFVG_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( EDBFVG_RC_SRCS ${EDBFVG_RCS} )
  
  # this will run uic on .ui files:
  QT4_WRAP_UI( EDBFVG_UI_HDRS ${EDBFVG_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( EDBFVG_MOC_SRCS ${EDBFVG_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
  # here we instruct CMake to build "edbfvg" executable from all of the source files
  ADD_EXECUTABLE( edbfvg ${EDBFVG_SRCS} ${EDBFVG_MOC_SRCS} ${EDBFVG_RC_SRCS} ${EDBFVG_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( edbfvg ${QT_LIBRARIES} )
  
  install(
    TARGETS
        edbfvg
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)

