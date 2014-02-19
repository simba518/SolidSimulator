SET(APPNAME set_elastic_material)

SET(NONE_QT_CPP
  Simulator.cpp
  ElasticMtlGroups.cpp
  main.cpp
)

SET(UI_SOURCES
  MainWindow.cpp  
)

SET(UI_HEADERS 

  PerturbationOp.h
  SelectionOp.h
  MainWindow.h
  SelectionOp.h
  DataModelRender.h
  DataModel.h
)

SET(UI_FORMS 
  set_elastic_mtl.ui
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
  /usr/local/lib/libutility_math.a
  /usr/local/lib/libutility_elastic.a

  ${QGLViewer_LIBRARIES}
  ${QT_LIBRARIES}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${Boost_LIBRARIES}

  ${ARPACK_LIBRARIES}
  ${SUPERLU2.0_LIBRARIES}
  /usr/lib/libblas.a
)