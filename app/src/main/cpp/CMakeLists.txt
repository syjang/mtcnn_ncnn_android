# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include/ncnn)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             FaceDetector.h
             FaceDetector.cpp

             det1.id.h
             det1.mem.h
             det2.id.h
             det2.mem.h
             det3.id.h
             det3.mem.h

             mtcnn.h
             mtcnn.cpp
             native-lib.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
set(NCNN_DIR D:/Project/faceRemover/app/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI})
#link_directories(${NCNN_DIR})
message(${NCNN_DIR})

target_link_libraries( # Specifies the target library.
                       native-lib
        PUBLIC
                       ${NCNN_DIR}/libncnn.a
                       ${OpenCV_LIBS}
                       ${log-lib} )