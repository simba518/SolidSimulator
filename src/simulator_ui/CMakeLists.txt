SET(APPNAME simulator_ui)

SET(NONE_QT_CPP
  main.cpp
  PassiveObject.cpp
)

SET(UI_SOURCES
  DragNodeOp.cpp
  DataModel.cpp
  MainWindow.cpp  
)

SET(UI_HEADERS 
  DragNodeOp.h
  PerturbationOp.h
  SelectionOp.h
  MainWindow.h
  SelectionOp.h
  DataModelRender.h
  DataModel.h
)

SET(UI_FORMS 
  simulator.ui
)

QT4_WRAP_CPP(UI_HEADERS_MOC ${UI_HEADERS})

SET(CMAKE_CURRENT_BINARY_DIR_RECORDED ${CMAKE_CURRENT_BINARY_DIR})
SET(CMAKE_CURRENT_BINARY_DIR  ${CMAKE_CURRENT_LIST_DIR})
QT4_WRAP_UI(UI_FORMS_HEADERS ${UI_FORMS})
SET(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR_RECORDED})

ADD_EXECUTABLE(${APPNAME}

  ${NONE_QT_CPP}
  ${UI_SOURCES} 
  ${UI_HEADERS_MOC}
  ${UI_FORMS_HEADERS}
)

TARGET_LINK_LIBRARIES(${APPNAME}

  ${LIBRARY_OUTPUT_PATH}/libsolid_simulator.a
  /usr/local/lib/libutility_ui.a
  /usr/local/lib/libutility_common.a
  /usr/local/lib/libutility_math.a
  /usr/local/lib/libutility_elastic.a

  ${QGLViewer_LIBRARIES}
  ${QT_LIBRARIES}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${Boost_LIBRARIES}

  ${ARPACK_LIBRARIES}
  ${SUITESPARSE_LIBRARIES}
  ${SUPERLU2.0_LIBRARIES}
  ${LAPACK_LIBRARIES}
)