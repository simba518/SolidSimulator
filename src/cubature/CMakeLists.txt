SET(APP_NAME cubature)

AUX_SOURCE_DIRECTORY(. cubature_src)
AUX_SOURCE_DIRECTORY(./cubacode cubacode_src)

ADD_EXECUTABLE(${APP_NAME} 
  ${cubature_src}
  ${cubacode_src}
  )

TARGET_LINK_LIBRARIES(${APP_NAME}
  
  /usr/local/lib/libutility_elastic.a
  /usr/local/lib/libutility_common.a
  ${Boost_LIBRARIES}
  )