#include "cian_plugin.h"

static void handle_vault_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_IN:  // amountIn
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_OUT:  // amountIn
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = OWNER;
            break;
        case OWNER:  // owner
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_deleverage_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case PROTOCOL_ID:  // lend protocol type
            context->next_param = WITHDRAW_TOKEN;
            break;
        case WITHDRAW_TOKEN:  // get token
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            printf_hex_array("context->token_received: ", ADDRESS_LENGTH, context->token_received);
            context->next_param = AMOUNT_OUT;
            break;
        case AMOUNT_OUT:  // Originally available quantity
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            printf_hex_array("context->amount_in_out: ", INT256_LENGTH, context->amount_in_out);
            context->next_param = UNUSED_OFFSET;
            break;
        case UNUSED_OFFSET:  // unused
            context->next_param = SWAPGET_MIN;
            break;
        case SWAPGET_MIN:  // Minimum amount to be exchanged
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = FLA_SELECTOR;
            printf_hex_array("context->amount_received: ", INT256_LENGTH, context->amount_received);
            break;
        case FLA_SELECTOR:  // Flash Loan Agreement Selector
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // owner
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            printf_hex_array("context->beneficiary: ", ADDRESS_LENGTH, context->beneficiary);
            context->next_param = OWNER;
            break;
        case OWNER:  // owner
            context->next_param = ONEINCH_LENGTH;
            break;
        case ONEINCH_LENGTH:  // 1inch bytes len
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->go_to_offset = true;
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_wrapper_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_IN:  // amountIn
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            printf_hex_array("context->amount_in_out: ", INT256_LENGTH, context->amount_in_out);
            context->next_param = UNUSED_OFFSET;
            break;
        case UNUSED_OFFSET:  // unused
            context->next_param = SWAPGET_MIN;
            break;
        case SWAPGET_MIN:  // Minimum amount to be exchanged
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = BENEFICIARY;
            printf_hex_array("context->amount_received: ", INT256_LENGTH, context->amount_received);
            break;
        case BENEFICIARY:  // owner
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            printf_hex_array("context->beneficiary: ", ADDRESS_LENGTH, context->beneficiary);
            context->next_param = ONEINCH_LENGTH;
            break;
        case ONEINCH_LENGTH:  // 1inch bytes len
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->go_to_offset = true;
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_wrapper_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_OUT:  // amountIn
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            printf_hex_array("context->amount_in_out: ", INT256_LENGTH, context->amount_in_out);
            context->next_param = UNUSED_OFFSET;
            break;
        case UNUSED_OFFSET:  // unused
            context->next_param = SWAPGET_MIN;
            break;
        case SWAPGET_MIN:  // Minimum amount to be exchanged
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = BENEFICIARY;
            printf_hex_array("context->amount_received: ", INT256_LENGTH, context->amount_received);
            break;
        case BENEFICIARY:  // owner
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            printf_hex_array("context->beneficiary: ", ADDRESS_LENGTH, context->beneficiary);
            context->next_param = IF_GET_WETH;
            break;
        case IF_GET_WETH:  // isWeth
            context->is_get_weth =
                (*(msg->parameter + PARAMETER_LENGTH - 1) == 0x01) ? true : false;
            PRINTF("context->is_get_weth bool %d\n", context->is_get_weth);
            printf_hex_array("context->is_get_weth: ", INT256_LENGTH, msg->parameter);
            context->next_param = ONEINCH_LENGTH;
            break;
        case ONEINCH_LENGTH:  // 1inch bytes len
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->go_to_offset = true;
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_wrapper_deposit_wsteth(ethPluginProvideParameter_t *msg,
                                                context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_IN:  // amountIn
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault_wrapper_withdraw_wsteth(ethPluginProvideParameter_t *msg,
                                                 context_t *context) {
    CHECK_AND_HANDLE_OFFSET(msg, context);

    switch (context->next_param) {
        case AMOUNT_OUT:  // amountOut
            copy_parameter(context->amount_in_out, msg->parameter, sizeof(context->amount_in_out));
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).

    PRINTF("plugin provide selectorIndex: %d\n", context->selectorIndex);
    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
            handle_vault_deposit(msg, context);
            break;
        case VAULT_WITHDRAW:
            handle_vault_withdraw(msg, context);
            break;
        case VAULT_DELEVERAGE_WITHDRAW:
            handle_vault_deleverage_withdraw(msg, context);
            break;
        case VAULT_WRAPPER_DEPOSIT:
            handle_vault_wrapper_deposit(msg, context);
            break;
        case VAULT_WRAPPER_WITHDRAW:
            handle_vault_wrapper_withdraw(msg, context);
            break;
        case VAULT_WRAPPER_DEPOSIT_WSTETH:
            handle_vault_wrapper_deposit_wsteth(msg, context);
            break;
        case VAULT_WRAPPER_WITHDRAW_WSTETH:
            handle_vault_wrapper_withdraw_wsteth(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
