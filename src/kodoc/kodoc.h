// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
    #if defined(KODOC_STATIC)
        // When building a static library, KODOC_API should be blank
        #define KODOC_API
    #elif defined(KODOC_DLL_EXPORTS)
        // When building the DLL, the API symbols must be exported
        #define KODOC_API __declspec(dllexport)
    #else
        // When a program uses the DLL, the API symbols must be imported
        #define KODOC_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        // When building a shared library, only the API symbols with the 'default'
        // visibility should be exported to hide all other symbols. All source
        // files should be compiled with the '-fvisibility=hidden' and
        // '-fvisibility-inlines-hidden' flags to achieve this.
        #define KODOC_API __attribute__ ((visibility ("default")))
    #else
        #define KODOC_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Callback function type used for tracing
typedef void (*kodoc_trace_callback_t)(const char*, const char*, void*);

//------------------------------------------------------------------
// KODO-C TYPES
//------------------------------------------------------------------

/// Opaque pointer used for the encoder and decoder factories
typedef struct kodoc_factory* kodoc_factory_t;

/// Opaque pointer used for encoders and decoders
typedef struct kodoc_coder* kodoc_coder_t;

/// Enum specifying the available finite fields
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    kodoc_binary,
    kodoc_binary4,
    kodoc_binary8
}
kodoc_finite_field;

/// Enum specifying the available codecs
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
/// Note: The codecs should also be listed in the wscript!
typedef enum
{
    kodoc_full_vector,
    kodoc_on_the_fly,
    kodoc_sliding_window,
    kodoc_sparse_full_vector,
    kodoc_seed,
    kodoc_sparse_seed,
    kodoc_perpetual,
    kodoc_fulcrum,
    kodoc_reed_solomon
}
kodoc_codec;

//------------------------------------------------------------------
// CONFIGURATION API
//------------------------------------------------------------------

/// Checks whether a given codec is available in the current configuration.
/// It is possible to enable or disable specific codecs when configuring kodo-c.
/// To see the relevant options, execute "python waf --help"
/// @param codec The codec type that should be checked
/// @return Non-zero value if the codec is available, otherwise 0
KODOC_API
uint8_t kodoc_has_codec(int32_t codec);

//------------------------------------------------------------------
// FACTORY API
//------------------------------------------------------------------

/// Builds a new encoder factory (for shallow storage encoders)
/// @param codec This parameter determines the encoding algorithms used.
/// @param finite_field The finite field that should be used by the encoder.
/// @param max_symbols The maximum number of symbols supported by encoders
///        built with this factory.
/// @param max_symbol_size The maximum symbol size in bytes supported by
///        encoders built using the returned factory
/// @return A new factory capable of building encoders using the
///         selected parameters.
KODOC_API
kodoc_factory_t kodoc_new_encoder_factory(
    int32_t codec, int32_t finite_field,
    uint32_t max_symbols, uint32_t max_symbol_size);

/// Builds a new decoder factory (for shallow storage decoders)
/// @param codec This parameter determines the decoding algorithms used.
/// @param finite_field The finite field that should be used by the decoder.
/// @param max_symbols The maximum number of symbols supported by decoders
///        built with this factory.
/// @param max_symbol_size The maximum symbol size in bytes supported by
///        decoders built using the returned factory
/// @return A new factory capable of building decoders using the
///         selected parameters.
KODOC_API
kodoc_factory_t kodoc_new_decoder_factory(
    int32_t codec, int32_t finite_field,
    uint32_t max_symbols, uint32_t max_symbol_size);

/// Deallocates and releases the memory consumed by a factory
/// @param factory The factory which should be deallocated
KODOC_API
void kodoc_delete_factory(kodoc_factory_t factory);

/// Returns the maximum number of symbols supported by the factory.
/// @param factory The factory to query
/// @return the maximum number of symbols
KODOC_API
uint32_t kodoc_factory_max_symbols(kodoc_factory_t factory);

/// Returns the maximum symbol size supported by the factory.
/// @param factory The factory to query
/// @return the maximum symbol size in bytes
KODOC_API
uint32_t kodoc_factory_max_symbol_size(kodoc_factory_t factory);

/// Returns the maximum block size supported by the factory.
/// @param factory The factory to query
/// @return The maximum amount of data encoded / decoded in bytes.
///         This is calculated by multiplying the maximum number of symbols
///         encoded / decoded by the maximum size of a symbol.
KODOC_API
uint32_t kodoc_factory_max_block_size(kodoc_factory_t factory);

/// Returns the maximum payload size supported by the factory.
/// @param factory The factory to query
/// @return the maximum required payload buffer size in bytes
KODOC_API
uint32_t kodoc_factory_max_payload_size(kodoc_factory_t factory);

/// Sets the number of symbols which should be used for the subsequent
/// encoders / decoders built with the specified factory. The value must
/// be below the max symbols used for the specific factory.
/// @param factory The factory which should be configured
/// @param symbols The number of symbols used for the next encoder/decoder
///        built with the factory.
KODOC_API
void kodoc_factory_set_symbols(kodoc_factory_t factory, uint32_t symbols);

/// Sets the symbol size which should be used for the subsequent
/// encoders / decoders built with the specified factory. The value must
/// be below the max symbol size used for the specific factory.
/// @param factory The factory which should be configured
/// @param symbol_size The symbol size used for the next encoder/decoder
///        built with the factory.
KODOC_API
void kodoc_factory_set_symbol_size(
    kodoc_factory_t factory, uint32_t symbol_size);

/// Builds a new encoder or decoder using the specified factory
/// @param factory The coder factory which should be used to build the coder
/// @return The new coder
KODOC_API
kodoc_coder_t kodoc_factory_build_coder(kodoc_factory_t factory);

/// Deallocates and releases the memory consumed by a coder
/// @param coder The coder which should be deallocated
KODOC_API
void kodoc_delete_coder(kodoc_coder_t coder);

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

/// Returns the payload size of an encoder/decoder, which is the maximum
/// size of a generated payload.
/// @param coder The encoder or decoder to query.
/// @return The required payload buffer size in bytes
KODOC_API
uint32_t kodoc_payload_size(kodoc_coder_t coder);

/// Reads the coded symbol in the payload buffer. The decoder state is
/// updated during this operation.
/// @param decoder The decoder to use.
/// @param payload The buffer storing the payload of an encoded symbol.
///        The payload buffer may be changed by this operation,
///        so it cannot be reused. If the payload is needed at several places,
///        make sure to keep a copy of the original payload.
KODOC_API
void kodoc_read_payload(kodoc_coder_t decoder, uint8_t* payload);

/// Writes a systematic/coded symbol into the provided payload buffer.
/// @param coder The encoder/decoder to use.
/// @param payload The buffer which should contain the (re/en)coded
///        symbol.
/// @return The total bytes used from the payload buffer
KODOC_API
uint32_t kodoc_write_payload(kodoc_coder_t coder, uint8_t* payload);

/// Checks whether the encoder/decoder provides the kodoc_write_payload()
/// function.
/// @param coder The encoder/decoder to query
/// @return Non-zero value if kodoc_write_payload is supported, otherwise 0
KODOC_API
uint8_t kodoc_has_write_payload(kodoc_coder_t coder);

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

/// Returns the block size of an encoder/decoder.
/// @param coder The coder to query.
/// @return The block size, i.e. the total size in bytes
///         that this coder operates on.
KODOC_API
uint32_t kodoc_block_size(kodoc_coder_t coder);

/// Specifies the source data for all symbols. This will specify all
/// symbols also in the case of partial data. If this is not desired,
/// then the symbols should be specified individually. This also
/// means that it is the responsibility of the user to communicate
/// how many of the bytes transmitted are application data.
/// @param encoder The encoder which will encode the data
/// @param data The buffer containing the data to be encoded
/// @param size The size of the buffer to be encoded
KODOC_API
void kodoc_set_const_symbols(
    kodoc_coder_t encoder, uint8_t* data, uint32_t size);

/// Specifies the source data for a given symbol.
/// @param encoder The encoder which will encode the symbol
/// @param index The index of the symbol in the coding block
/// @param data The buffer containing the data to be encoded
/// @param size The size of the symbol buffer
KODOC_API
void kodoc_set_const_symbol(
    kodoc_coder_t encoder, uint32_t index, uint8_t* data, uint32_t size);

/// Specifies the data buffer where the decoder should store the decoded
/// symbols. This will specify the storage for all symbols. If this is not
/// desired, then the symbols can be specified individually.
/// @param decoder The decoder which will decode the data
/// @param data The buffer that should contain the decoded symbols
/// @param size The size of the buffer to be decoded
KODOC_API
void kodoc_set_mutable_symbols(
    kodoc_coder_t decoder, uint8_t* data, uint32_t size);

/// Specifies the data buffer where the decoder should store a given symbol.
/// @param decoder The decoder which will decode the symbol
/// @param index The index of the symbol in the coding block
/// @param data The buffer that should contain the decoded symbol
/// @param size The size of the symbol buffer
KODOC_API
void kodoc_set_mutable_symbol(
    kodoc_coder_t decoder, uint32_t index, uint8_t* data, uint32_t size);

/// Returns the symbol size of an encoder/decoder.
/// @param coder The encoder/decoder to check
/// @return The size of a symbol in bytes
KODOC_API
uint32_t kodoc_symbol_size(kodoc_coder_t coder);

/// Returns the number of symbols in a block (i.e. the generation size).
/// @param coder The encoder/decoder to check
/// @return The number of symbols
KODOC_API
uint32_t kodoc_symbols(kodoc_coder_t coder);

/// Returns the size of coefficient vector.
/// @param coder The encoder/decoder to check
/// @return The size of coefficient vector
KODOC_API
uint32_t kodoc_coefficient_vector_size(kodoc_coder_t coder);

//------------------------------------------------------------------
// CODEC API
//------------------------------------------------------------------

/// Checks whether decoding is complete.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is complete, otherwise 0
KODOC_API
uint8_t kodoc_is_complete(kodoc_coder_t decoder);

/// Check whether the decoder supports partial decoding. This means
/// means that the decoder will be able to decode symbols on-the-fly.
/// If the decoder supports the partial decoding tracker, then the
/// kodoc_is_partially_complete() function can be used to determine if some of
/// the symbols are fully decoded.
/// @param coder The decoder to query
/// @return Non-zero if the decoder supports partial decoding, otherwise 0
KODOC_API
uint8_t kodoc_has_partial_decoding_interface(kodoc_coder_t decoder);

/// Check whether decoding is partially complete. This means that some
/// symbols in the decoder are fully decoded. You can use the
/// kodoc_is_symbol_uncoded() function to determine which symbols.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is partially complete, otherwise 0
KODOC_API
uint8_t kodoc_is_partially_complete(kodoc_coder_t decoder);

/// The rank of a decoder indicates how many symbols have been decoded
/// or partially decoded. The rank of an encoder indicates how many symbols
/// are available for encoding.
/// @param coder The coder to query
/// @return The rank of the decoder or encoder
KODOC_API
uint32_t kodoc_rank(kodoc_coder_t coder);

/// Checks whether the encoder or decoder can use/provide feedback information.
/// The encoder can disregard some symbols if the feedback from decoder
/// indicates that those symbols were already decoded.
/// @param coder The encoder/decoder to query
/// @return Non-zero value if feedback is supported, otherwise 0
KODOC_API
uint8_t kodoc_has_feedback_size(kodoc_coder_t coder);

/// Returns the feedback size of an encoder/decoder.
/// @param coder The encoder/decoder to check
/// @return The size of the required feedback buffer in bytes
KODOC_API
uint8_t kodoc_feedback_size(kodoc_coder_t coder);

/// Reads the feedback information from the provided buffer.
/// @param encoder The encoder to use.
/// @param feedback The buffer which contains the feedback information
KODOC_API
void kodoc_read_feedback(kodoc_coder_t encoder, uint8_t* feedback);

/// Writes the feedback information into the provided buffer.
/// @param decoder The decoder to use.
/// @param feedback The buffer which should contain the feedback information.
/// @return The total bytes used from the feeback buffer
KODOC_API
uint32_t kodoc_write_feedback(kodoc_coder_t decoder, uint8_t* feedback);

/// Indicates if a symbol is partially or fully decoded. A symbol with
/// a pivot element is defined in the coding matrix of a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is defined, otherwise 0
KODOC_API
uint8_t kodoc_is_symbol_pivot(kodoc_coder_t decoder, uint32_t index);

/// Indicates whether a symbol is missing at a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is missing, otherwise 0
KODOC_API
uint8_t kodoc_is_symbol_missing(kodoc_coder_t decoder, uint32_t index);

/// Indicates whether a symbol has been partially decoded at a decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol has been partially decoded,
///         otherwise 0
KODOC_API
uint8_t kodoc_is_symbol_partially_decoded(
    kodoc_coder_t decoder, uint32_t index);

/// Indicates whether a symbol is available in an uncoded (i.e. fully decoded)
/// form at the decoder.
/// @param decoder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is uncoded, otherwise 0
KODOC_API
uint8_t kodoc_is_symbol_uncoded(kodoc_coder_t decoder, uint32_t index);

/// Returns the number of missing symbols.
/// @param decoder The decoder to query
/// @return The number of missing symbols at the decoder
KODOC_API
uint32_t kodoc_symbols_missing(kodoc_coder_t decoder);

/// Returns the number of partially decoded symbols.
/// @param decoder The decoder to query
/// @return The number of partially decoded symbols at the decoder
KODOC_API
uint32_t kodoc_symbols_partially_decoded(kodoc_coder_t decoder);

/// Returns the number of uncoded (i.e. fully decoded) symbols.
/// @param decoder The decoder to query
/// @return The number of uncoded symbols at the decoder
KODOC_API
uint32_t kodoc_symbols_uncoded(kodoc_coder_t decoder);

/// Returns whether an decoder implements the
/// symbol_decoding_status_updater_interface
/// @param encoder The decoder
/// @return Non-zero if the decoder implements the
///         symbol_decoding_status_updater_interface, otherwise 0
KODOC_API
uint8_t kodoc_has_symbol_decoding_status_updater_interface(
    kodoc_coder_t decoder);

/// Sets the status updater on.
/// @param decoder The decoder to modify
KODOC_API
void kodoc_set_status_updater_on(kodoc_coder_t decoder);

/// Sets the status updater off.
/// @param decoder The decoder to modify
KODOC_API
void kodoc_set_status_updater_off(kodoc_coder_t decoder);

/// Updates the symbol status so that all uncoded symbols, label partially
/// decoded, will labelled as uncoded.
/// @param decoder The decoder to update
KODOC_API
void kodoc_update_symbol_status(kodoc_coder_t decoder);

/// Returns whether the symbol status updater is enabled or not.
/// @param decoder The decoder to query
/// @return Non-zero value if the symbol status updater is enabled, otherwise 0
KODOC_API
uint8_t kodoc_is_status_updater_enabled(kodoc_coder_t decoder);

/// Reads and decodes an encoded symbol according to the provided coding
/// coefficients.
/// @param decoder The decoder to use.
/// @param symbol_data The encoded symbol
/// @param coefficients The coding coefficients that were used to
///        calculate the encoded symbol
KODOC_API
void kodoc_read_symbol(
    kodoc_coder_t decoder, uint8_t* symbol_data, uint8_t* coefficients);

/// Reads and decodes a systematic/uncoded symbol with the corresponding
/// symbol index.
/// @param decoder The decoder to use.
/// @param symbol_data The uncoded source symbol.
/// @param index The index of this uncoded symbol in the data block.
KODOC_API
void kodoc_read_uncoded_symbol(
    kodoc_coder_t decoder, uint8_t* symbol_data, uint32_t index);

/// Writes an encoded symbol according to the provided symbol coefficients.
/// @param encoder The encoder to use.
/// @param symbol_data The destination buffer for the encoded symbol
/// @param coefficients The desired coding coefficients that should
///        be used to calculate the encoded symbol.
/// @return The number of bytes used.
KODOC_API
uint32_t kodoc_write_symbol(
    kodoc_coder_t encoder, uint8_t* symbol_data, uint8_t* coefficients);

/// Writes a systematic/uncoded symbol that corresponds to the provided
/// symbol index.
/// @param encoder The encoder to use.
/// @param symbol_data The destination of the uncoded source symbol.
/// @param index The index of this uncoded symbol in the data block.
/// @return The number of bytes used.
KODOC_API
uint32_t kodoc_write_uncoded_symbol(
    kodoc_coder_t encoder, uint8_t* symbol_data, uint32_t index);

//------------------------------------------------------------------
// SYSTEMATIC API
//------------------------------------------------------------------

/// Returns whether an encoder has systematic capabilities
/// @param encoder The encoder
/// @return Non-zero if the encoder supports the systematic mode, otherwise 0
KODOC_API
uint8_t kodoc_has_systematic_interface(kodoc_coder_t encoder);

/// Returns whether the encoder is in the systematic mode, i.e. if it will
/// initially send the original source symbols with a simple header.
/// @param encoder The encoder
/// @return Non-zero if the encoder is in the systematic mode, otherwise 0
KODOC_API
uint8_t kodoc_is_systematic_on(kodoc_coder_t encoder);

/// Switches the systematic encoding on
/// @param encoder The encoder
KODOC_API
void kodoc_set_systematic_on(kodoc_coder_t encoder);

/// Switches the systematic encoding off
/// @param encoder The encoder
KODOC_API
void kodoc_set_systematic_off(kodoc_coder_t encoder);

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

/// Returns whether an encoder or decoder supports the trace interface
/// @param coder The encoder/decoder to query
/// @return Non-zero value if tracing is supported, otherwise 0
KODOC_API
uint8_t kodoc_has_trace_interface(kodoc_coder_t coder);

/// Enables the trace function of the encoder/decoder, which prints
/// to the standard output.
/// @param coder The encoder/decoder to use
KODOC_API
void kodoc_set_trace_stdout(kodoc_coder_t coder);

/// Registers a custom callback that will get the trace output of an encoder
/// or decoder. The function takes a void pointer which will be available when
/// the kodoc_trace_callback_t function is invoked by the library. This allows
/// the user to pass custom information to the callback function. If no context
/// is needed the pointer can be set to NULL.
/// @param coder The encoder/decoder to use
/// @param callback The callback that processes the trace output
/// @param context A void pointer which is forwarded to the callback function.
///        This can be used when state is required within the callback. If no
///        state is needed the pointer can be set to NULL.
KODOC_API
void kodoc_set_trace_callback(
    kodoc_coder_t coder, kodoc_trace_callback_t callback, void* context);

/// Disables the trace function of the encoder/decoder.
/// @param coder The encoder/decoder to use
KODOC_API
void kodoc_set_trace_off(kodoc_coder_t coder);

/// Sets the zone prefix that should be used for the trace output of
/// a particular encoder/decoder instance. The zone prefix can help to
/// differentiate the output that is coming from various coder instances.
/// @param coder The encoder/decoder to use
/// @param prefix The zone prefix for the trace output
KODOC_API
void kodoc_set_zone_prefix(kodoc_coder_t coder, const char* prefix);

//------------------------------------------------------------------
// SPARSE ENCODER API
//------------------------------------------------------------------

/// Returns the current coding vector density of a sparse encoder.
/// @param coder The encoder to query
/// @return The coding vector density (0.0 < density <= 1.0)
KODOC_API
double kodoc_density(kodoc_coder_t encoder);

/// Sets the coding vector density of a sparse encoder.
/// @param encoder The encoder to use
/// @param density The density value (0.0 < density <= 1.0)
KODOC_API
void kodoc_set_density(kodoc_coder_t encoder, double density);

//------------------------------------------------------------------
// PERPETUAL ENCODER API
//------------------------------------------------------------------

/// Get the pseudo-systematic property of the perpetual generator
/// @param encoder The encoder to use
/// @return the current setting for pseudo-systematic
KODOC_API
uint8_t kodoc_pseudo_systematic(kodoc_coder_t encoder);

/// Set the pseudo-systematic property of the perpetual generator
/// @param encoder The encoder to use
/// @param pseudo_systematic the new setting for pseudo-systematic
KODOC_API
void kodoc_set_pseudo_systematic(
    kodoc_coder_t encoder, uint8_t pseudo_systematic);

/// Get the pre-charging property of the perpetual generator
/// @param encoder The encoder to use
/// @return the current setting for pre-charging
KODOC_API
uint8_t kodoc_pre_charging(kodoc_coder_t encoder);

/// Set the pre-charging property of the perpetual generator
/// @param encoder The encoder to use
/// @param pre_charging the new setting for pre-charging
KODOC_API
void kodoc_set_pre_charging(kodoc_coder_t encoder, uint8_t pre_charging);

/// Get the width (the number of non-zero coefficients after the pivot)
/// @param encoder The encoder to use
/// @return the width used by the perpetual generator
KODOC_API
uint32_t kodoc_width(kodoc_coder_t encoder);

/// Set the perpetual width, i.e. the number of non-zero coefficients after the
/// pivot. The width_ratio is calculated from this value.
/// @param encoder The encoder to use
/// @param width the perpetual width (0 <= width < symbols)
KODOC_API
void kodoc_set_width(kodoc_coder_t encoder, uint32_t width);

/// Get the ratio that is used to calculate the width
/// @param encoder The encoder to use.
/// @return the width ratio of the perpetual generator
KODOC_API
double kodoc_width_ratio(kodoc_coder_t encoder);

/// Set the ratio that is used to calculate the number of non-zero
/// coefficients after the pivot (i.e. the width)
/// @param encoder The encoder to use
/// @param width_ratio the width ratio (0.0 < width_ratio <= 1.0)
KODOC_API
void kodoc_set_width_ratio(kodoc_coder_t encoder, double width_ratio);

//------------------------------------------------------------------
// FULCRUM CODER API
//------------------------------------------------------------------

/// Get the number of expansion symbols on a fulcrum coder
/// @param coder The coder to use
KODOC_API
uint32_t kodoc_expansion(kodoc_coder_t coder);

/// Get the number of inner symbols on a fulcrum coder
/// @param coder The coder to use
KODOC_API
uint32_t kodoc_inner_symbols(kodoc_coder_t coder);

//------------------------------------------------------------------
// FULCRUM ENCODER API
//------------------------------------------------------------------

/// Get the number of nested symbols on a fulcrum encoder
/// @param encoder The encoder to use
KODOC_API
uint32_t kodoc_nested_symbols(kodoc_coder_t encoder);

/// Get the number of nested symbol_size on a fulcrum encoder
/// @param encoder The encoder to use
KODOC_API
uint32_t kodoc_nested_symbol_size(kodoc_coder_t encoder);

//------------------------------------------------------------------
// FULCRUM FACTORY API
//------------------------------------------------------------------

/// Get the maximum number of expansion symbols for a fulcrum factory
/// @param factory The factory to use
KODOC_API
uint32_t kodoc_factory_max_expansion(kodoc_factory_t factory);

/// Get the maximum number of inner symbols for a fulcrum factory
/// @param factory The factory to use
KODOC_API
uint32_t kodoc_factory_max_inner_symbols(kodoc_factory_t factory);

/// Set the number of expansion symbols for a fulcrum factory
/// @param factory The factory to use
/// @param expansion The number of expansion symbols to use
KODOC_API
void kodoc_factory_set_expansion(kodoc_factory_t factory, uint32_t expansion);

#ifdef __cplusplus
}
#endif
