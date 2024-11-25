# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/v5.3.1/esp-idf/components/bootloader/subproject"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/tmp"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/src"
  "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/pedro/USP/8_Semestre/embarcados/SEL0337---Projetos-em-Sistemas-Embarcados/Pratica5/Parte2/HC_SR04_BLE/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
