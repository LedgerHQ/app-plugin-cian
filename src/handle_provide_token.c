#include "cian_plugin.h"

void handle_provide_token(ethPluginProvideInfo_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    PRINTF("CIAN plugin provide token: 0x%p, 0x%p\n", msg->item1, msg->item2);
    if (msg->item1) {
        // The Ethereum App found the information for the requested token!
        // Store its decimals.
        context->decimals = msg->item1->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker, (char *) msg->item1->token.ticker, sizeof(context->ticker));

        // Keep track that we found the token.
        context->token_found = true;
    } else {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found = false;

        // If we wanted to add a screen, say a warning screen for example, we could instruct the
        // ethereum app to add an additional screen by setting `msg->additionalScreens` here, just
        // like so:
        // msg->additionalScreens = 1;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
