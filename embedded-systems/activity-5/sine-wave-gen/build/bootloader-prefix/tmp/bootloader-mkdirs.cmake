# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/nix/store/6zr22cvda693zzrkq8rfy6wh2d54891a-esp-idf-v5.3.1/components/bootloader/subproject"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/tmp"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/src/bootloader-stamp"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/src"
  "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/locochoco/embedded-systems-and-microprocessor-classes/embedded-systems/activity-5/sine-wave-gen/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
