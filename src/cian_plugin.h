#pragma once

#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS 7

// Name of the plugin.
#define PLUGIN_NAME "CIAN"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
typedef enum {
    VAULT_DEPOSIT = 0,
    VAULT_WITHDRAW,
    VAULT_DELEVERAGE_WITHDRAW,
    VAULT_WRAPPER_DEPOSIT,
    VAULT_WRAPPER_WITHDRAW,
    VAULT_WRAPPER_DEPOSIT_WSTETH,   // only for eth mainnet
    VAULT_WRAPPER_WITHDRAW_WSTETH,  // only for eth mainnet
    UNUSED
} selector_t;

// Enumeration used to parse the smart contract data.
typedef enum {
    MIN_AMOUNT_RECEIVED = 0,
    TOKEN_RECEIVED,
    BENEFICIARY,
    UNUSED_OFFSET,
    ONEINCH_LENGTH,
    AMOUNT_IN,
    AMOUNT_OUT,
    PROTOCOL_ID,
    WITHDRAW_TOKEN,
    SWAPGET_MIN,
    FLA_SELECTOR,
    IF_GET_WETH,
    OWNER,
    UNEXPECTED_PARAMETER,
} parameter;

// Rename `CIAN` to be the same as the one initialized in `main.c`.
extern const uint32_t CIAN_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_t {
    // For display.
    uint8_t amount_received[INT256_LENGTH];
    uint8_t amount_in_out[INT256_LENGTH];
    uint8_t beneficiary[ADDRESS_LENGTH];
    uint8_t token_received[ADDRESS_LENGTH];
    bool is_get_weth;
    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t token_found;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);

static inline void printf_hex_array(const char *title __attribute__((unused)),
                                    size_t len __attribute__((unused)),
                                    const uint8_t *data __attribute__((unused))) {
    PRINTF(title);
    for (size_t i = 0; i < len; ++i) {
        PRINTF("%02x", data[i]);
    };
    PRINTF("\n");
}

#define CHECK_AND_HANDLE_OFFSET(msg_ptr, context_ptr)                              \
    if ((context_ptr)->go_to_offset) {                                             \
        if ((msg_ptr)->parameterOffset != (context_ptr)->offset + SELECTOR_SIZE) { \
            return;                                                                \
        }                                                                          \
        (context_ptr)->go_to_offset = false;                                       \
    }

typedef enum {
    VAULT_SEND_SCREEN,
    VAULT_WITHDRAW_SCREEN,
    VAULT_RECEIVE_TOKEN_SCREEN,
    WARAPPER_SEND_SCREEN1,
    WARAPPER_SEND_SCREEN2,
    WARAPPER_DEPOSIT_SWAPTO_SCREEN,
    WARAPPER_WITHDRAW_SWAPTO_SCREEN,
    WARAPPER_DEPOSIT_WSTETH_SCREEN,
    WARAPPER_WITHDRAW_WSTETH_SCREEN,
    BENEFICIARY_SCREEN,
    WARNING_SCREEN,
    ERROR,
} screens_t;

#define CIAN_MAX_TICKER_LEN 20
#define SELECTOR_UI_NUM     13
#define TOKEN_TICKER_NUM    9
#define MAX_SCREEN_NUM      4

typedef struct {
    uint8_t selectorIndex;
    uint8_t *contract_address;
    char ticker[CIAN_MAX_TICKER_LEN];
} cian_ui_info_t;

typedef struct {
    uint8_t token_address[ADDRESS_LENGTH];
    char token_name[CIAN_MAX_TICKER_LEN];
} token_ticker_t;

extern const uint8_t ETHEREUM_STETH_VAULT_ADDR[ADDRESS_LENGTH];
extern const uint8_t ETHEREUM_STETH_VAULT_WRAPPER_ADDR[ADDRESS_LENGTH];
extern const uint8_t ETHEREUM_RETH_VAULT_ADDR[ADDRESS_LENGTH];
extern const uint8_t ETHEREUM_RETH_VAULT_WRAPPER_ADDR[ADDRESS_LENGTH];
extern const uint8_t ARBITRUM_WSTETH_VAULT_ADDR[ADDRESS_LENGTH];
extern const uint8_t ARBITRUM_WSTETH_VAULT_WRAPPER_ADDR[ADDRESS_LENGTH];
extern const uint8_t OPTIMISM_WSTETH_VAULT_ADDR[ADDRESS_LENGTH];
extern const uint8_t OPTIMISM_WSTETH_VAULT_WRAPPER_ADDR[ADDRESS_LENGTH];
extern const token_ticker_t TOKEN_TICKER_COLLECTION[TOKEN_TICKER_NUM];