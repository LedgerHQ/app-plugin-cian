#include "cian_plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:  /* fall through */
        case VAULT_WITHDRAW: /* fall through */
        case VAULT_WRAPPER_DEPOSIT_WSTETH:
            msg->numScreens = 1;
            break;
        case VAULT_DELEVERAGE_WITHDRAW: /* fall through */
        case VAULT_WRAPPER_DEPOSIT:
            msg->numScreens = 3;
            break;
        case VAULT_WRAPPER_WITHDRAW: /* fall through */
        case VAULT_WRAPPER_WITHDRAW_WSTETH:
            msg->numScreens = 2;
            break;
        default:
            PRINTF("Handle_finalize missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    // Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the beneficiary is NOT the sender, we will need an additional screen to display it.
    if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH) != 0) {
        msg->numScreens += 1;
    }

    // Set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    msg->tokenLookup1 = context->token_received;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
