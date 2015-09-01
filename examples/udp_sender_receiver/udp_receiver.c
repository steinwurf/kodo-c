// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <stdlib.h>
#include <kodoc/kodoc.h>

#ifdef _WIN32
    #include <winsock2.h>
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

// Count the total number of packets received in order to decode
unsigned int rx_packets;

static void exit_on_sigint(int sig)
{
    (void) sig;
    printf("\nTotal number of received packets: %d\n", rx_packets);
    exit(0);
}


int main(int argc, char* argv[])
{
    // Variables needed for the network / socket usage
    int32_t socket_descriptor = 0;
    int32_t return_code = 0;
    int32_t bytes_received = 0;
    socklen_t remote_address_size;
    struct sockaddr_in remote_address;
    struct sockaddr_in local_address;

    // Variables needed for the coding
    uint32_t max_symbols = 32;
    uint32_t max_symbol_size = 160;

    uint32_t symbols = 0;

    int32_t code_type = kodo_on_the_fly;
    int32_t finite_field = kodo_binary8;

    kodo_factory_t decoder_factory = 0;
    kodo_coder_t decoder = 0;

    // The buffer used to receive incoming packets
    uint32_t payload_size = 0;
    uint8_t* payload = 0;

    // Keeps track of which symbols have been decoded
    uint8_t* decoded = (uint8_t*) malloc(sizeof(uint8_t) * max_symbols);

    // Initialize winsock if on Windows
#ifdef _WIN32

    WORD versionWanted = MAKEWORD(1, 1);
    WSADATA wsaData;

    return_code = WSAStartup(versionWanted, &wsaData);

    if (return_code != 0)
    {
        // Tell the user that we could not find a usable
        // Winsock DLL.
        printf("WSAStartup failed with error: %d\n", return_code);
        exit(1);
    }

#endif

    // Initialize global variables
    rx_packets = 0;

    if (argc < 3)
    {
        printf("usage : %s <port> <symbols>\n", argv[0]);
        exit(1);
    }

    // Socket creation
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor < 0)
    {
        printf("%s: cannot open socket \n", argv[0]);
        exit(1);
    }

    // Bind local server port
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(atoi(argv[1]));
    return_code = bind(socket_descriptor, (struct sockaddr*) &local_address,
                       sizeof(local_address));

    if (return_code < 0)
    {
        printf("%s: cannot bind port number %d \n", argv[0], atoi(argv[1]));
        exit(1);
    }

    // Install signal handler
    signal(SIGINT, exit_on_sigint);

    // Initialize the factory with the chosen symbols and symbol size
    symbols = atoi(argv[2]);
    if (symbols > max_symbols)
    {
        printf("%s: number of symbols cannot be higher than %d \n",
               argv[0], max_symbols);
        exit(1);
    }

    // Create the encoder factory
    decoder_factory = kodo_new_decoder_factory(code_type, finite_field,
                                               max_symbols, max_symbol_size,
                                               kodo_trace_enabled,
                                               kodo_deep_storage);

    kodo_factory_set_symbols(decoder_factory, symbols);
    decoder = kodo_factory_new_decoder(decoder_factory);

    // Create the buffer needed for the payload
    payload_size = kodo_payload_size(decoder);
    payload = (uint8_t*) malloc(payload_size);

    // Zero initialize the decoded array */
    memset(decoded, '\0', sizeof(uint8_t) * max_symbols);

    printf("%s: waiting for data on UDP port %u\n", argv[0], atoi(argv[1]));

    // Receiver loop
    while (!kodo_is_complete(decoder))
    {
        // Receive message
        remote_address_size = sizeof(remote_address);

        bytes_received = recvfrom(
            socket_descriptor, payload, payload_size, 0,
            (struct sockaddr*) &remote_address, &remote_address_size);

        if (bytes_received < 0)
        {
            printf("%s: recvfrom error %d\n", argv[0], bytes_received);
            fflush(stdout);
            continue;
        }

        // Print received message
        printf("%s: UDP packet from %s:%u : %d\n",
               argv[0],inet_ntoa(remote_address.sin_addr),
               ntohs(remote_address.sin_port), bytes_received);

        ++rx_packets;

        // Packet got through - pass that packet to the decoder
        kodo_read_payload(decoder, payload);

        if (kodo_has_partial_decoding_tracker(decoder) &&
            kodo_is_partial_complete(decoder))
        {
            uint32_t i = 0;
            for (; i < kodo_symbols(decoder); ++i)
            {
                if (!kodo_is_symbol_uncoded(decoder, i))
                    continue;

                if (!decoded[i])
                {
                    // Update that this symbol now has been decoded,
                    // in a real application we could copy out the symbol
                    // using the kodo_copy_from_symbol(..)
                    printf("Symbol %d was decoded\n", i);
                    decoded[i] = 1;
                }
            }
        }
    }

    printf("Data decoded!\n");

    // Cleanup
    free(decoded);
    free(payload);

    kodo_delete_decoder(decoder);
    kodo_delete_decoder_factory(decoder_factory);

    return 0;
}
