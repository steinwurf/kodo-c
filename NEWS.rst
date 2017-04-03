News for kodo-c
===============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* tbd

11.0.0
------
* Major: Upgrade to waf-tools 4
* Major: Upgrade to kodo-core 8
* Major: Upgrade to kodo-rlnc 6
* Major: Upgrade to kodo-fulcrum 6
* Major: Upgrade to kodo-reed-solomon 7
* Minor: Upgrade to gtest 4

10.0.0
------
* Major: Upgrade to kodo-core 6
* Major: Upgrade to kodo-rlnc 5
* Major: Upgrade to kodo-fulcrum 5
* Major: Upgrade to kodo-reed-solomon 5

9.0.0
-----
* Major: Upgrade to kodo-core 4
* Major: Upgrade to kodo-rlnc 4
* Major: Upgrade to kodo-fulcrum 4
* Major: Upgrade to kodo-reed-solomon 4

8.0.1
-----
* Patch: Disable the rlnc and fulcrum API when the corresponding repositories
  are not available.

8.0.0
-----
* Major: The ``kodo_`` prefix was changed to ``kodoc_`` in all functions,
  types and enums. The old prefix caused name collisions with symbols defined
  in the main kodo repositories.
* Major: Replaced ``kodo_factory_new_encoder`` and ``kodo_factory_new_decoder``
  with ``kodo_factory_build_coder``.
* Minor: Added the ``enable_codecs`` option to only enable specific codecs
  when configuring kodo-c. This option can be used to build a smaller library.
* Major: Depend on kodo-core, kodo-rlnc, kodo-fulcrum and kodo-reed-solomon
  instead of kodo (kodo has been split to multiple repositories).
* Major: Replaced ``kodo_has_set_trace_callback``, ``kodo_has_set_trace_stdout``
  and ``kodo_has_set_trace_off `` with ``kodo_has_trace_interface``.
* Major: Replaced ``kodo_has_set_systematic_off`` with
  ``kodo_has_systematic_interface``.
* Major: Renamed ``kodo_has_partial_decoding_tracker`` to
  ``kodo_has_partial_decoding_interface``.
* Minor: Added the Reed-Solomon codec (``kodo_reed_solomon`` code type).
* Major: Upgrade to kodo 34
* Major: Renamed ``kodo_is_partial_complete`` to ``kodo_is_partially_complete``.
* Major: Renamed ``kodo_symbols_seen`` to ``kodo_symbols_partially_decoded``.

7.0.1
-----
* Patch: Removed the post() call on the task generator of the shared library,
  which led to problems in dependent projects.

7.0.0
-----
* Minor: Completely rewrote the C bindings backend to use Kodo's new
  polymorphic API.
* Minor: Added fulcrum stack.
* Major: Renamed ``kodo_delete_encoder_factory`` and
  ``kodo_delete_decoder_factory`` to ``kodo_delete_factory``.
* Major: Renamed ``kodo_delete_encoder`` and ``kodo_delete_decoder`` to
  ``kodo_delete_coder``.
* Major: Upgrade to kodo 33
* Major: Upgrade to waf-tools 3
* Minor: Upgrade to gtest 3
* Major: Removed support for deep storage (shallow storage is used for all
  codecs).
* Major: The trace API is available in all codecs (option removed).
* Major: Split ``kodo_set_symbol`` to const and mutable variants:
  ``kodo_set_const_symbol`` and ``kodo_set_mutable_symbol``.
* Major: Split ``kodo_set_symbols`` to const and mutable variants:
  ``kodo_set_const_symbols`` and ``kodo_set_mutable_symbols``.
* Major: Removed ``copy_from_symbol`` and ``copy_from_symbols`` since they
  are not used with shallow storage.

6.0.0
-----
* Major: Renamed files to follow the name of the class defined in the files.
* Major: Upgrade to kodo 30.
* Minor: Added perpetual stack.
* Major: Renamed enum values ``kodo_full_rlnc``, ``kodo_sparse_seed_rlnc``,
  ``kodo_seed_rlnc`` to ``kodo_full_vector``, ``kodo_sparse_seed``,
  ``kodo_seed`` respectively, to better follow the Kodo API.
* Major: Added context pointer to trace callback.

5.0.0
-----
* Major: Renamed ``copy_symbol`` and ``copy_symbols`` to
  ``copy_from_symbol`` and ``copy_from_symbols`` to their name in kodo 29.
* Major: Upgrade to kodo 29
* Major: Upgrade to fifi 20
* Major: Renamed `trace` and `trace_callback` to the appropriate names from the
  new kodo version.
* Major: Upgrade to kodo 27
* Minor: Added buildbot.py for coverage reports.
* Minor: Added ``kodo_has_write_payload`` to check if a coder provides the
  ``kodo_write_payload`` function.

4.0.0
-----
* Major: Upgrade to kodo 26
* Major: Renamed ``kodo_symbol_pivot`` to ``kodo_is_symbol_pivot`` to match the
  function name in Kodo.

3.1.0
-----
* Minor: Added the ``kodo_sparse_seed_rlnc`` code type that implements a sparse
  seed-based RLNC codec. The code density should be set on the encoder side.
  The maximum size of the coding header is 9 bytes.
* Minor: Moved the factory functions from ``kodoc.cpp`` to separate translation
  units in order to decrease the compilation time. The optimizer might use
  gigabytes of RAM when instantiating a lot of coder stack variants in a
  single cpp file. This problem is mitigated with this separation.
* Minor: Added a seed-based RLNC codec (``kodo_seed_rlnc``).

3.0.0
-----
* Minor: Added sparse RLNC encoder.
* Major: Upgrade to kodo 25
* Major: Upgrade to fifi 19
* Minor: Added simple throughput benchmark in ``benchmark/kodoc_throughput``
* Minor: Extended the C API with functions for reading and writing symbols
  directly on the decoder and encoder.
* Minor: Added a shallow full RLNC codec.
* Major: Upgrade to sak 14
* Minor: Re-enabled the build task for the kodo-c static library.

2.0.0
-----
* Major: Restructured the API according to the Hourglass pattern that was
  presented at CppCon 2014: https://www.youtube.com/watch?v=PVYdHDm0q6Y
  This pattern encapsulates the complexity of the underlying C++ libraries
  by hiding all implementation details behind a simple C89 API. It makes
  providing bindings from other languages easier, facilitates integration with
  other C++ projects and build systems, and insulates from ABI issues.
* Minor: Update to waf 1.8.0-pre1
* Minor: Made python files comply with pep8
* Major: Upgrade to kodo 21
* Major: Upgrade to fifi 15
* Major: Upgrade to sak 13
* Major: Added recycle dependency
* Patch: The kodo_is_symbol_decoded() function call was forwarded to the
  wrong kodo function.
* Major: Removed debug functions (Replaced by trace layers)
* Minor: Add trace layer functions
* Minor: Added the sliding window algorithm
* Minor: Added new examples:
    sliding_window,
    switch_systematic_on_off
    use_trace_layers
* Minor: Updated examples:
    encode_decode_simple
    encode_decode_on_the_fly
    udp_sender_receiver
* Minor: Implemented method kodo_read_feedback
* Minor: Implemented method kodo_write_feedback

1.7.0
-----
* Minor: Bump dependency to Kodo version 14.x.y

1.6.0
-----
* Minor: Bump dependency to Kodo version 13.x.y

1.5.0
-----
* Minor: Added API for creating debug stacks and printing debug information.

1.4.0
-----
* Minor: Added API to control systematic encoding. Systematic encoding means
  that all source symbols will be sent once un-coded before coding begins.

1.3.0
-----
* Minor: Bump dependency to Kodo version 12.x.y
* Minor: Improve on-the-fly encoding / decoding support

1.2.1
-----
* Patch: Allow compilation without using a makespec

1.2.0
-----
* Minor: Added support for on-the-fly encoding and a corresponding example.
* Patch: Setting c++11 compiler flag to -std=c++0x in the makefile/Makefile

1.1.0
-----
* Minor: Added the standalone archiver Waf tool. The tool makes it possible to
  create self contained archives of the Kodo C bindings i.e. it contains all
  needed dependencies.

1.0.0
-----
* Major: Initial version of the Kodo C bindings based on Kodo version 11.x.y.
