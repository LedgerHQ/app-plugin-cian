#include "cian_plugin.h"
#include "plugin_utils.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    size_t index;
    if (!find_selector(U4BE(msg->selector, 0), CIAN_SELECTORS, NUM_SELECTORS, &index)) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }
    context->selectorIndex = index;

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
            context->next_param = AMOUNT_IN;
            break;
        case VAULT_WITHDRAW:
            context->next_param = AMOUNT_OUT;
            break;
        case VAULT_DELEVERAGE_WITHDRAW:
            context->next_param = PROTOCOL_ID;
            break;
        case VAULT_WRAPPER_DEPOSIT:
        case VAULT_WRAPPER_DEPOSIT_WSTETH:
            context->next_param = AMOUNT_IN;
            break;
        case VAULT_WRAPPER_WITHDRAW:
        case VAULT_WRAPPER_WITHDRAW_WSTETH:
            context->next_param = AMOUNT_OUT;
            break;
        // Keep this
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
