cmake_minimum_required(VERSION 3.30...4.0 FATAL_ERROR)

#
#Attacus
#
add_subdirectory(attacus)

#
#AttacusPy
#
if(${HAS_PY})
add_subdirectory(attacus_py)
endif() #HAS_PY
