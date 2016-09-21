// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <stdint.h>
#include <stdlib.h>
#include <kodoc/kodoc.h>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#ifdef _WIN32

#include <windows.h>

void sleep_here(uint32_t milliseconds)
{
    Sleep(milliseconds);
}

#else

void sleep_here(uint32_t milliseconds)
{
    usleep(milliseconds * 1000); // takes microseconds
}

#endif


int main(int argc, char* argv[])
{
    // Variables needed for the network / socket usage
    int32_t socket_descriptor;
    int32_t return_code;
    uint32_t i;

    struct sockaddr_in local_address;
    struct sockaddr_in remote_address;
    struct hostent* host;

    uint32_t delay = 0; // Delay between packets

    // Variables needed for the coding
    uint32_t max_symbols = 32;
    uint32_t max_symbol_size = 160;

    uint32_t symbols = 0;
    uint32_t packets = 0;

    int32_t codec = kodoc_on_the_fly;
    int32_t finite_field = kodoc_binary8;

    kodoc_factory_t encoder_factory = 0;
    kodoc_coder_t encoder = 0;

    // The buffer sent to the receiver
    uint32_t payload_size = 0;
    uint8_t* payload = 0;
    uint32_t bytes_used = 0;

    // The data to be encoded
    uint32_t block_size = 0;
    uint8_t* data_in = 0;

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

    // Check command line args
    if (argc != 6)
    {
        printf("usage : %s <server> <port> <symbols> <packets> <delay_ms>\n",
               argv[0]);

        exit(1);
    }

    // Get the delay
    delay = atol(argv[5]);
    printf("Delay is: %u milliseconds\n", delay);

    // Get server IP address (no check if input is IP address or DNS name)
    host = gethostbyname(argv[1]);
    if (host == NULL)
    {
        printf("%s: unknown host '%s' \n", argv[0], argv[1]);
        exit(1);
    }

    printf("Sending data to '%s:%d' (IP: %s) \n", host->h_name,
           atoi(argv[2]), inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));

    remote_address.sin_family = host->h_addrtype;
    memcpy((char*) &remote_address.sin_addr.s_addr,
           host->h_addr_list[0], host->h_length);
    remote_address.sin_port = htons(atoi(argv[2]));

    // Socket creation
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor < 0)
    {
        printf("%s: cannot open socket \n", argv[0]);
        exit(1);
    }

    // Bind any port
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(0);

    return_code = bind(socket_descriptor, (struct sockaddr*) &local_address,
                       sizeof(local_address));

    if (return_code < 0)
    {
        printf("%s: cannot bind port\n", argv[0]);
        exit(1);
    }

    // Create the encoder factory
    encoder_factory = kodoc_new_encoder_factory(codec, finite_field,
                                                max_symbols, max_symbol_size);

    // Initialize the factory with the chosen symbols and symbol size
    symbols = atoi(argv[3]);
    if (symbols > max_symbols)
    {
        printf("%s: number of symbols cannot be higher than %d \n",
               argv[0], max_symbols);
        exit(1);
    }
    packets = atoi(argv[4]);
    if (packets < symbols)
    {
        printf("%s: number of packets should be higher than %d \n",
               argv[0], symbols);
    }

    kodoc_factory_set_symbols(encoder_factory, symbols);
    encoder = kodoc_factory_build_coder(encoder_factory);

    // Create the buffer needed for the payload
    payload_size = kodoc_payload_size(encoder);
    payload = (uint8_t*)malloc(payload_size);

    // Create some data to encode
    block_size = kodoc_block_size(encoder);
    data_in = (uint8_t*)malloc(block_size);

    for (i = 0; i < block_size; ++i)
        data_in[i] = rand() % 256;

    // Send data
    for (i = 0; i < packets; ++i)
    {
        if (kodoc_rank(encoder) < kodoc_symbols(encoder))
        {
            // The rank of an encoder indicates how many symbols have
            // been added, i.e. how many symbols are available for encoding
            uint32_t rank = kodoc_rank(encoder);

            // Calculate the offset to the next symbol to insert
            uint8_t* symbol = data_in + (rank * kodoc_symbol_size(encoder));
            kodoc_set_const_symbol(
                encoder, rank, symbol, kodoc_symbol_size(encoder));
        }

        bytes_used = kodoc_write_payload(encoder, payload);
        printf("Payload generated by encoder, rank = %d, bytes used = %d\n",
               kodoc_rank(encoder), bytes_used);

        return_code = sendto(socket_descriptor, payload, bytes_used, 0,
                             (struct sockaddr*) &remote_address,
                             sizeof(remote_address));

        if (return_code < 0)
        {
            printf("%s: cannot send data %d \n", argv[0], i-1);
            close(socket_descriptor);
            exit(1);
        }

        sleep_here(delay);
    }

    // Clean up
    free(data_in);
    free(payload);

    kodoc_delete_coder(encoder);
    kodoc_delete_factory(encoder_factory);

    return 0;
}
