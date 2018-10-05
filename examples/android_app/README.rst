Android Application
-------------------

This folder contains an Android Studio demo project for including kodo-c
in an Android application.

First we need to configure and build kodo-c using the Android NDK. Go to the
root of the kodo-c repository and run the following commands::

  python waf configure --cxx_mkspec=cxx_android5_clang50_armv7 \
  --android_sdk_dir={android-sdk} --android_ndk_dir={android-ndk}
  python waf build

Where ``{android-sdk}`` and ``{android-ndk}`` should be replaced with your
path to the Android SDK and a standalone version of the Android NDK.

After building kodo-c with the specified mkspec, you can install the generated
static libraries to ``app/src/main/jni/libs/armeabi-v7a``.
It is also recommended to copy the ``kodoc.h`` header to
``app/src/main/jni/kodoc/kodoc.h``.
After this, you need to adjust the build settings in
``app/src/main/jni/Android.mk``. The preconfigured
solution shows how to link with the static libraries: ``kodoc fifi cpuid``.

.. note:: If you want to use the generated static libraries with ``ndk-build``,
          then make sure that you process at least one C++ source file (.cpp)
          with ``ndk-build`` (this might be a dummy cpp file). Otherwise you
          will get a lot of linkage issues, because ``ndk-build`` does not link
          with the C++ standard library by default.

Creating a new project
......................

If you want to use kodo-c in a new project, then create a project using the
Android Studio wizard.

Then add the path to the Android NDK to ``local.properties``.

Right click in the Project panel, then select: New -> Folder -> JNI Folder.

This will create a folder called ``C`` in the project panel
(note: the actual name of this folder on the filesystem is ``jni``).

Go to this newly generated directory and use ``javah`` to generate the
JNI headers, using the following command::

    javah -cp {android-sdk}/platforms/android-16/android.jar:../java {class}

Replace ``{android-sdk}`` with the path to the Android SDK on your
machine, and ``{class}`` with the name of the class. In the ``dummy_android``
sample project, this would be ``com.steinwurf.dummy_android.MainActivity``.
You can generate JNI headers for any class in your project.

