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
typedef void (*kodo_trace_callback_t)(const char*, const char*, void*);

//------------------------------------------------------------------
// FACTORY API
//------------------------------------------------------------------

/// Opaque pointer used for the encoder and decoder factories
typedef struct kodo_factory* kodo_factory_t;

/// Opaque pointer used for encoders and decoders
typedef struct kodo_coder* kodo_coder_t;

/// Enum specifying the available finite fields
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    kodo_binary,
    kodo_binary4,
    kodo_binary8
}
kodo_finite_field;

/// Enum specifying the available code types
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    kodo_full_vector,
    kodo_on_the_fly,
    kodo_sliding_window,
    kodo_sparse_full_vector,
    kodo_seed,
    kodo_sparse_seed,
    kodo_perpetual,
    kodo_fulcrum
}
kodo_code_type;

/// Enum specifying the available trace modes
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    kodo_trace_disabled,
    kodo_trace_enabled
}
kodo_trace_mode;

/// Enum specifying the available storage modes
/// Note: the size of the enum type cannot be guaranteed, so the int32_t type
/// is used in the API calls to pass the enum values
typedef enum
{
    kodo_deep_storage,
    kodo_shallow_storage
}
kodo_storage_mode;

//------------------------------------------------------------------
// FACTORY API
//------------------------------------------------------------------

/// Builds a new encoder factory (for deep storage encoders)
/// @param code_type This parameter determines the encoding algorithms used.
/// @param finite_field The finite field that should be used by the encoder.
/// @param max_symbols The maximum number of symbols supported by encoders
///        built with this factory.
/// @param max_symbol_size The maximum symbol size in bytes supported by
///        encoders built using the returned factory
/// @param trace_mode Determines which trace mode should be used.
/// @param storage_mode @todo
/// @return A new factory capable of building encoders using the
///         selected parameters.
KODOC_API
kodo_factory_t kodo_new_encoder_factory(
    int32_t code_type, int32_t finite_field,
    uint32_t max_symbols, uint32_t max_symbol_size);

/// Builds a new decoder factory (for deep storage decoders)
/// @param code_type This parameter determines the decoding algorithms used.
/// @param finite_field The finite field that should be used by the decoder.
/// @param max_symbols The maximum number of symbols supported by decoders
///        built with this factory.
/// @param max_symbol_size The maximum symbol size in bytes supported by
///        decoders built using the returned factory
/// @param trace_mode Determines which trace mode should be used.
/// @param storage_mode @todo
/// @return A new factory capable of building decoders using the
///         selected parameters.
KODOC_API
kodo_factory_t kodo_new_decoder_factory(
    int32_t code_type, int32_t finite_field,
    uint32_t max_symbols, uint32_t max_symbol_size);

/// Deallocates and releases the memory consumed by the encoder factory
/// @param factory The encoder factory which should be deallocated
KODOC_API
void kodo_delete_encoder_factory(kodo_factory_t factory);

/// Deallocates and releases the memory consumed by the decoder factory
/// @param factory The decoder factory which should be deallocated
KODOC_API
void kodo_delete_decoder_factory(kodo_factory_t factory);

/// Returns the maximum number of symbols supported by the factory.
/// @param factory The factory to query
/// @return the maximum number of symbols
KODOC_API
uint32_t kodo_factory_max_symbols(kodo_factory_t factory);

/// Returns the maximum symbol size supported by the factory.
/// @param factory The factory to query
/// @return the maximum symbol size in bytes
KODOC_API
uint32_t kodo_factory_max_symbol_size(kodo_factory_t factory);

/// Returns the maximum block size supported by the factory.
/// @param factory The factory to query
/// @return The maximum amount of data encoded / decoded in bytes.
///         This is calculated by multiplying the maximum number of symbols
///         encoded / decoded by the maximum size of a symbol.
KODOC_API
uint32_t kodo_factory_max_block_size(kodo_factory_t factory);

/// Returns the maximum payload size supported by the factory.
/// @param factory The factory to query
/// @return the maximum required payload buffer size in bytes
KODOC_API
uint32_t kodo_factory_max_payload_size(kodo_factory_t factory);

/// Sets the number of symbols which should be used for the subsequent
/// encoders / decoders built with the specified factory. The value must
/// be below the max symbols used for the specific factory.
/// @param factory The factory which should be configured
/// @param symbols The number of symbols used for the next encoder/decoder
///        built with the factory.
KODOC_API
void kodo_factory_set_symbols(kodo_factory_t factory, uint32_t symbols);

/// Sets the symbol size which should be used for the subsequent
/// encoders / decoders built with the specified factory. The value must
/// be below the max symbols used for the specific factory.
/// @param factory The factory which should be configured
/// @param symbol_size The symbol size used for the next encoder/decoder
///        built with the factory.
KODOC_API
void kodo_factory_set_symbol_size(kodo_factory_t factory,
                                  uint32_t symbol_size);

/// Builds a new encoder using the specified factory
/// @param factory The encoder factory which should be used to
///        build the encoder
/// @return The new encoder
KODOC_API
kodo_coder_t kodo_factory_new_encoder(kodo_factory_t factory);

/// Builds a new decoder using the specified factory
/// @param factory The decoder factory which should be used to
///        build the decoder
/// @return The new decoder
KODOC_API
kodo_coder_t kodo_factory_new_decoder(kodo_factory_t factory);

/// Deallocates and releases the memory consumed by an encoder
/// @param encoder The encoder which should be deallocated
KODOC_API
void kodo_delete_encoder(kodo_coder_t encoder);

/// Deallocates and releases the memory consumed by a decoder
/// @param decoder The decoder which should be deallocated
KODOC_API
void kodo_delete_decoder(kodo_coder_t decoder);

//------------------------------------------------------------------
// PAYLOAD API
//------------------------------------------------------------------

/// Returns the payload size of an encoder/decoder, which is the maximum
/// size of a generated payload.
/// @param coder The encoder or decoder to query.
/// @return The required payload buffer size in bytes
KODOC_API
uint32_t kodo_payload_size(kodo_coder_t coder);

/// Reads the coded symbol in the payload buffer. The decoder state is
/// updated during this operation.
/// @param decoder The decoder to use.
/// @param payload The buffer storing the payload of an encoded symbol.
///        The payload buffer may be changed by the decode function,
///        an cannot be reused. If the payload is needed in several places
///        make sure to keep a copy of the original payload.
KODOC_API
void kodo_read_payload(kodo_coder_t decoder, uint8_t* payload);

/// Writes a systematic/coded symbol into the provided payload buffer.
/// @param coder The encoder/decoder to use.
/// @param payload The buffer which should contain the (re/en)coded
///        symbol.
/// @return The total bytes used from the payload buffer
KODOC_API
uint32_t kodo_write_payload(kodo_coder_t coder, uint8_t* payload);

/// Checks whether the encoder/decoder provides the kodo_write_payload()
/// function.
/// @param coder The encoder/decoder to query
/// @return Non-zero value if kodo_write_payload is supported, otherwise 0
KODOC_API
uint8_t kodo_has_write_payload(kodo_coder_t coder);

//------------------------------------------------------------------
// SYMBOL STORAGE API
//------------------------------------------------------------------

/// Returns the block size of an encoder/decoder.
/// @param coder The coder to query.
/// @return The block size, i.e. the total size in bytes
///         that this coder operates on.
KODOC_API
uint32_t kodo_block_size(kodo_coder_t coder);

/// Specifies the source data for all symbols. This will specify all
/// symbols also in the case of partial data. If this is not desired,
/// then the symbols should be specified individually. This also
/// means that it is the responsibility of the user to communicate
/// how many of the bytes transmitted are application data.
/// @param encoder The encoder which will encode the data
/// @param data The buffer containing the data to be encoded
/// @param size The size of the buffer to be encoded
KODOC_API
void kodo_set_const_symbols(kodo_coder_t encoder, uint8_t* data, uint32_t size);

/// Specifies the source data for a given symbol.
/// @param encoder The encoder which will encode the data
/// @param index The index of the symbol in the coding block
/// @param data The buffer containing the data to be encoded
/// @param size The size of the symbol buffer
KODOC_API
void kodo_set_const_symbol(kodo_coder_t encoder, uint32_t index, uint8_t* data,
    uint32_t size);

KODOC_API
void kodo_set_mutable_symbols(kodo_coder_t encoder, uint8_t* data,
    uint32_t size);

KODOC_API
void kodo_set_mutable_symbol(kodo_coder_t encoder, uint32_t index,
    uint8_t* data, uint32_t size);

/// Copies the decoded symbols to the provided buffer.
/// @param decoder The decoder which contains the data to be
///        copied.
/// @param data The destination buffer to which the data should be copied
/// @param size The size of the data to be copied
KODOC_API
void kodo_copy_from_symbols(kodo_coder_t decoder, uint8_t* data, uint32_t size);

/// Copies a specific symbol to the provided buffer.
/// @param decoder The decoder which contains the data to be
///        copied.
/// @param index The index of the symbol to copy
/// @param data The destination buffer to which the data should be copied
/// @param size The size of the data to be copied
KODOC_API
void kodo_copy_from_symbol(kodo_coder_t decoder, uint32_t index,
                           uint8_t* data, uint32_t size);

/// Returns the symbol size of an encoder/decoder.
/// @param coder The encoder/decoder to check
/// @return The size of a symbol in bytes
KODOC_API
uint32_t kodo_symbol_size(kodo_coder_t coder);

/// Returns the number of symbols in a block (i.e. the generation size).
/// @param coder The encoder/decoder to check
/// @return The number of symbols
KODOC_API
uint32_t kodo_symbols(kodo_coder_t coder);

//------------------------------------------------------------------
// CODEC API
//------------------------------------------------------------------

/// Checks whether decoding is complete.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is complete, otherwise 0
KODOC_API
uint8_t kodo_is_complete(kodo_coder_t decoder);

/// Check whether decoding is partially complete. This means that some
/// symbols in the decoder are fully decoded. You can use the
/// kodo_is_symbol_uncoded() function to determine which symbols.
/// @param decoder The decoder to query
/// @return Non-zero value if the decoding is partially complete, otherwise 0
KODOC_API
uint8_t kodo_is_partial_complete(kodo_coder_t decoder);

/// The rank of a decoder indicates how many symbols have been decoded
/// or partially decoded. The rank of an encoder indicates how many symbols
/// are available for encoding.
/// @param coder The coder to query
/// @return The rank of the decoder or encoder
KODOC_API
uint32_t kodo_rank(kodo_coder_t coder);

/// Checks whether the encoder or decoder can use/provide feedback information.
/// The encoder can disregard some symbols if the feedback from decoder
/// indicates that those symbols were already decoded.
/// @param coder The encoder/decoder to query
/// @return Non-zero value if feedback is supported, otherwise 0
KODOC_API
uint8_t kodo_has_feedback_size(kodo_coder_t coder);

/// Returns the feedback size of an encoder/decoder.
/// @param coder The encoder/decoder to check
/// @return The size of the required feedback buffer in bytes
KODOC_API
uint8_t kodo_feedback_size(kodo_coder_t coder);

/// Reads the feedback information from the provided buffer.
/// @param encoder The encoder to use.
/// @param feedback The buffer which contains the feedback information
KODOC_API
void kodo_read_feedback(kodo_coder_t encoder, uint8_t* feedback);

/// Writes the feedback information into the provided buffer.
/// @param decoder The decoder to use.
/// @param feedback The buffer which should contain the feedback information.
/// @return The total bytes used from the feeback buffer
KODOC_API
uint32_t kodo_write_feedback(kodo_coder_t decoder, uint8_t* feedback);

/// Indicates whether a symbol is defined in the coding matrix
/// of an encoder/decoder. A symbol with a pivot element might not be fully
/// decoded in the coding matrix of a decoder, therefore use the
/// kodo_is_symbol_uncoded() function to check if a symbol is fully decoded.
/// @param coder The encoder/decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is defined, otherwise 0
KODOC_API
uint8_t kodo_is_symbol_pivot(kodo_coder_t coder, uint32_t index);

/// Indicates whether a symbol is available in an uncoded (i.e. fully decoded)
/// form in an encoder or decoder.
/// @param coder The decoder to query
/// @param index Index of the symbol whose state should be checked
/// @return Non-zero value if the symbol is uncoded, otherwise 0
KODOC_API
uint8_t kodo_is_symbol_uncoded(kodo_coder_t decoder, uint32_t index);

/// Returns the number of uncoded symbols.
/// @param coder The decoder to query
/// @return The number of uncoded symbols in the decoder
KODOC_API
uint32_t kodo_symbols_uncoded(kodo_coder_t decoder);

/// Returns the number of seen symbols.
/// @param coder The decoder to query
/// @return The number of symbols seen by the decoder. The seen symbols might
///         not be fully decoded.
KODOC_API
uint32_t kodo_symbols_seen(kodo_coder_t decoder);

/// Reads and decodes an encoded symbol according to the coding
/// coefficients stored in the corresponding symbol_id.
/// @param decoder The decoder to use.
/// @param symbol_data The encoded symbol
/// @param coefficients The coding coefficients used to
///        create the encoded symbol
KODOC_API
void kodo_read_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                      uint8_t* coefficients);

/// Reads and decodes a systematic/uncoded symbol with the corresponding
/// symbol index.
/// @param decoder The decoder to use.
/// @param symbol_data The uncoded source symbol.
/// @param index The index of this uncoded symbol in the data block.
KODOC_API
void kodo_read_uncoded_symbol(kodo_coder_t decoder, uint8_t* symbol_data,
                              uint32_t index);

/// Writes an encoded symbol according to the provided symbol coefficients.
/// @param encoder The encoder to use.
/// @param symbol_data The destination buffer for the encoded symbol
/// @param coefficients At this point the symbol id should be
///        initialized with the desired coding coefficients.
/// @return The number of bytes used.
KODOC_API
uint32_t kodo_write_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                           uint8_t* coefficients);

/// Writes a systematic/uncoded symbol that corresponds to the provided
/// symbol index.
/// @param encoder The encoder to use.
/// @param symbol_data The destination of the uncoded source symbol.
/// @param index The index of this uncoded symbol in the data block.
/// @return The number of bytes used.
KODOC_API
uint32_t kodo_write_uncoded_symbol(kodo_coder_t encoder, uint8_t* symbol_data,
                                   uint32_t index);

//------------------------------------------------------------------
// SPARSE ENCODER API
//------------------------------------------------------------------

/// Returns the current coding vector density of an encoder.
/// @param coder The encoder to query.
/// @return The coding vector density (0.0 < density <= 1.0)
KODOC_API
double kodo_density(kodo_coder_t encoder);

/// Sets the coding vector density of an encoder.
/// @param encoder The encoder to use.
/// @param density The density value (0.0 < density <= 1.0)
KODOC_API
void kodo_set_density(kodo_coder_t encoder, double density);

//------------------------------------------------------------------
// PERPETUAL ENCODER API
//------------------------------------------------------------------

/// Get the pseudo-systematic property of the generator
/// @param encoder The encoder to use.
/// @return the current setting for pseudo-systematic
KODOC_API
uint8_t kodo_pseudo_systematic(kodo_coder_t encoder);

/// Set the pseudo-systematic property of the generator
/// @param encoder The encoder to use.
/// @param pseudo_systematic the new setting for pseudo-systematic
KODOC_API
void kodo_set_pseudo_systematic(kodo_coder_t encoder,
    uint8_t pseudo_systematic);

/// Get the pre-charging property of the generator
/// @param encoder The encoder to use.
/// @return the current setting for pre-charging
KODOC_API
uint8_t kodo_pre_charging(kodo_coder_t encoder);

/// Set the pre-charging property of the generator
/// @param encoder The encoder to use.
/// @param pre_charging the new setting for pre-charging
KODOC_API
void kodo_set_pre_charging(kodo_coder_t encoder, uint8_t pre_charging);

/// Get the width
/// @param encoder The encoder to use.
/// @return the width used by the generator
KODOC_API
uint32_t kodo_width(kodo_coder_t encoder);

/// Set the number of non-zero coefficients after the pivot.
/// Width ratio is recalculated from this value
/// @param encoder The encoder to use.
/// @param width the width
KODOC_API
void kodo_set_width(kodo_coder_t encoder, uint32_t width);

/// Get the ratio that is used to calculate the width
/// @param encoder The encoder to use.
/// @return the width ratio of the generator
KODOC_API
double kodo_width_ratio(kodo_coder_t encoder);

/// Set the ratio that is used to calculate the number of non-zero
/// coefficients after the pivot (i.e. the width)
/// @param encoder The encoder to use.
/// @param ratio the width ratio
KODOC_API
void kodo_set_width_ratio(kodo_coder_t encoder, double width_ratio);

//------------------------------------------------------------------
// GENERIC API
//------------------------------------------------------------------

/// Check whether the decoder supports partial decoding. This means
/// means that the decoder will be able to decode symbols on-the-fly.
/// If the decoder supports the partial decoding tracker, then the
/// kodo_is_partial_complete() function can be used to determine if some of
/// the symbols are fully decoded and therefore can be copied out of the
/// decoder.
/// @param coder The decoder to query
/// @return Non-zero if the decoder supports partial decoding, otherwise 0
KODOC_API
uint8_t kodo_has_partial_decoding_tracker(kodo_coder_t decoder);

/// Returns whether an encoder has systematic capabilities
/// @param encoder The encoder
/// @return Non-zero if the encoder supports the systematic mode, otherwise 0
KODOC_API
uint8_t kodo_has_set_systematic_off(kodo_coder_t encoder);

/// Returns whether the encoder is in the systematic mode, i.e. if it will
/// initially send the original source symbols with a simple header.
/// @param encoder The encoder
/// @return Non-zero if the encoder is in the systematic mode, otherwise 0
KODOC_API
uint8_t kodo_is_systematic_on(kodo_coder_t encoder);

/// Switches the systematic encoding on
/// @param encoder The encoder
KODOC_API
void kodo_set_systematic_on(kodo_coder_t encoder);

/// Switches the systematic encoding off
/// @param encoder The encoder
KODOC_API
void kodo_set_systematic_off(kodo_coder_t encoder);

//------------------------------------------------------------------
// TRACE API
//------------------------------------------------------------------

/// Returns whether an encoder or decoder has set trace callback capabilities
/// @param coder The encoder/decoder to query
/// @return Non-zero value if tracing is supported, otherwise 0
KODOC_API
uint8_t kodo_has_set_trace_callback(kodo_coder_t coder);

/// Returns whether an encoder or decoder has stdout trace capabilities
/// @param coder The encoder/decoder to query
/// @return Non-zero value if tracing is supported, otherwise 0
KODOC_API
uint8_t kodo_has_set_trace_stdout(kodo_coder_t coder);

/// Returns whether an encoder or decoder has set trace off capabilities
/// @param coder The encoder/decoder to query
/// @return Non-zero value if tracing is supported, otherwise 0
KODOC_API
uint8_t kodo_has_set_trace_off(kodo_coder_t coder);

/// Enables the trace function of the encoder/decoder, which prints
/// to the standard output.
/// @param coder The encoder/decoder to use
KODOC_API
void kodo_set_trace_stdout(kodo_coder_t coder);

/// Registers a custom callback that will get the trace output of an encoder
/// or decoder. The function takes a void pointer which will be available when
/// the kodo_trace_callback_t function is invoked by the library. This allows
/// the user to pass custom information to the callback function. If no context
/// is needed the pointer can be set to NULL.
/// @param coder The encoder/decoder to use
/// @param callback The callback that processes the trace output
/// @param context A void pointer which is forwarded to the callback function.
///        This can be used when state is required within the callback. If no
///        state is needed the pointer can be set to NULL.
KODOC_API
void kodo_set_trace_callback(kodo_coder_t coder, kodo_trace_callback_t callback,
    void* context);

/// Disables the trace function of the encoder/decoder.
/// @param coder The encoder/decoder to use
KODOC_API
void kodo_set_trace_off(kodo_coder_t coder);

#ifdef __cplusplus
}
#endif
