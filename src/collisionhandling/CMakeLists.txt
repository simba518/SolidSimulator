SET(APP_NAME collisionhandling)

AUX_SOURCE_DIRECTORY(. collisionhandling_src)

ADD_EXECUTABLE(${APP_NAME} 
  ${collisionhandling_src}
  )

TARGET_LINK_LIBRARIES(${APP_NAME}
  
  /usr/local/lib/libutility_elastic.a
  /usr/local/lib/libutility_common.a
  /usr/local/lib/libutility_math.a
  ${LAPACK_LIBRARIES}
  ${SUITESPARSE_LIBRARIES}
  /usr/lib/libblas.so
  # /usr/lib/libblas.a
  ${Boost_LIBRARIES}
  )