iOS Demo Application
--------------------

The ``kodoc-ios-demo`` folder contains an iOS demo project that can be opened
in XCode. This project uses the iOS multi-arch libraries that can be compiled
by running the following helper scripts::

    cd examples/ios_app
    bash configure_and_build_all.sh
    bash build_ios_fat_libs.sh

This project should work on all architectures (device and simulator)
after you execute the helper scripts.

If you want to include kodo-c in your own Xcode project, then please set
the library and include path options as follows:

- Specify the library path and dependencies in "Link Binary with Libraries"
  under "Build Phases" in the project navigator.
- Specify the include path in "Header Search Paths" under "Search Paths",
  in the "Build Settings".

Manual compilation
..................

The helper scripts provided above can automate the build process, but you
can also configure and compile kodo-c manually for your desired architecture by
executing the following commands from the root of the kodo-c repository::

    python waf configure --cxx_mkspec=cxx_ios70_apple_llvm_{arch}
    python waf build
    python waf install --install_path=/tmp/{arch} --install_static_libs

The ``{arch}`` placeholder defines the target architecture. Currently
``armv7``, ``armv7s``, ``arm64``, ``i386``, and ``x86_64`` are available
(the latter two are needed for the iOS simulator builds).

The ``install_path`` option determines where the static libraries will be
installed. Here, we install the static libraries to ``/tmp/{arch}``. This
means 5 target folders for the 5 supported architectures.

After completing the above steps for each architecture, the static libraries
can be combined into a "fat", multi-arch static library using the ``lipo``
command::

  lipo -create /tmp/{arch1}/libkodoc_static.a /tmp/{arch2}/libkodoc_static.a \
  -output libkodoc_static.a

Several input libraries can be included in the multi-arch lib, e.g. all the
above-mentioned architectures.

You can use the ``file`` command to check if all the desired architectures
are included in the fat library::

    file libkodoc_static.a

This should output something like this::

    libkodoc_static.a: Mach-O universal binary with 5 architectures
    libkodoc_static.a (for architecture i386):      current ar archive random library
    libkodoc_static.a (for architecture x86_64):    current ar archive random library
    libkodoc_static.a (for architecture armv7):     current ar archive random library
    libkodoc_static.a (for architecture armv7s):    current ar archive random library
    libkodoc_static.a (for architecture arm64):     current ar archive random library
