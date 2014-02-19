SET(LIBRARY_NAME solid_simulator)

AUX_SOURCE_DIRECTORY(. solid_simulator_src)

ADD_LIBRARY(${LIBRARY_NAME} ${solid_simulator_src})