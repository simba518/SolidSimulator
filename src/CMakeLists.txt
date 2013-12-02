SET(APPNAME SimulatorUI)

SET(NONE_QT_CPP
  main.cpp
)

SET(UI_SOURCES
  MainWindow.cpp  
)

SET(UI_HEADERS 

  MainWindow.h
  SelectionOp.h
  DataModel.h
)

SET(UI_FORMS 
  simulator.ui
)

QT4_WRAP_CPP(UI_HEADERS_MOC ${UI_HEADERS})
QT4_WRAP_UI(UI_FORMS_HEADERS ${UI_FORMS})

ADD_EXECUTABLE(${APPNAME}

  ${NONE_QT_CPP}
  ${UI_SOURCES} 
  ${UI_HEADERS_MOC}
  ${UI_FORMS_HEADERS}
)

TARGET_LINK_LIBRARIES(${APPNAME}
 
  /usr/local/lib/libutility_ui.a
  /usr/local/lib/libutility_common.a
  ${QGLViewer_LIBRARIES}
  ${QT_LIBRARIES}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${Boost_LIBRARIES}
)