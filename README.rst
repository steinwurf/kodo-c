License
-------

A valid Kodo license is required if you wish to use this project.

Please request a license by **filling out the license request** form_.

Kodo is available under a research- and education-friendly license,
you can see the details here_.

If you try to configure without a valid license, then you will get an error!

.. _form: http://steinwurf.com/license/
.. _here: http://steinwurf.com/research-license/

About
-----

**Warning!**: The kodo-c library is now deprecated and it will not receive
any updates in the future. Please consider using the following repositories
for future development:

  - https://github.com/steinwurf/kodo-rlnc-c
  - https://github.com/steinwurf/kodo-rlnc
  - https://github.com/steinwurf/kodo-reed-solomon
  - https://github.com/steinwurf/kodo-fulcrum
  - https://github.com/steinwurf/kodo-perpetual

This repository contains high-level C bindings for the Kodo erasure coding
library. The bindings provide access to the basic functionality provided by
Kodo, such as encoding and decoding data. The examples folder provides
sample applications that demonstrate the usage of the C API.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=kodo-c
    :target: http://buildbot.steinwurf.dk/stats?projects=kodo-c

If you have any questions or suggestions about this library, please contact
us at our developer mailing list (hosted at Google Groups):

* http://groups.google.com/group/steinwurf-dev

Documentation
-------------

To get started, see the manual here:
http://docs.steinwurf.com

Quick Start
-----------

If you already installed a C++11 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/kodo-c.git

Configure and build the project::

    cd kodo-c
    python waf configure
    python waf build

Run the unit tests::

  python waf --run_tests

Special Options
---------------

With the ``enable_codecs`` option, you can configure kodo-c to only enable
some desired codecs and disable all others. For example::

    python waf configure --enable_codecs=full_vector

Run ``python waf --help`` to list the available codecs. You can even
select multiple codecs with a comma-separated list::

    python waf configure --enable_codecs=full_vector,seed,sparse_seed
