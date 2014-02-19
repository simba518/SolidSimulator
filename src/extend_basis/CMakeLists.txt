SET(APP_NAME extend_basis)

AUX_SOURCE_DIRECTORY(. extend_basis_src)

ADD_EXECUTABLE(${APP_NAME} 
  ${extend_basis_src}
  )

TARGET_LINK_LIBRARIES(${APP_NAME}
  
  /usr/local/lib/libutility_elastic.a
  /usr/local/lib/libutility_common.a
  /usr/local/lib/libutility_math.a
  ${ARPACK_LIBRARIES}
  ${SUPERLU2.0_LIBRARIES}
  ${LAPACK_LIBRARIES}
  ${SUITESPARSE_LIBRARIES}
  /usr/lib/libblas.a
  ${Boost_LIBRARIES}
  )