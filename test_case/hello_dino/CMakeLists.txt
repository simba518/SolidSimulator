SET(APP_NAME hello_dino)

AUX_SOURCE_DIRECTORY(. hello_dino_src)

ADD_EXECUTABLE(${APP_NAME}
  ${hello_dino_src}
  )

TARGET_LINK_LIBRARIES(${APP_NAME}

  ${LIBRARY_OUTPUT_PATH}/libsolid_simulator.a
  /usr/local/lib/libutility_elastic.a
  /usr/local/lib/libutility_math.a
  /usr/local/lib/libutility_common.a

  ${Boost_LIBRARIES}
  ${ARPACK_LIBRARIES}
  ${SUITESPARSE_LIBRARIES}
  ${SUPERLU2.0_LIBRARIES}
  ${LAPACK_LIBRARIES}
)