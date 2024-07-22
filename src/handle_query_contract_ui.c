#include "cian_plugin.h"

static void set_send_ui(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgScreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_DEPOSIT) {
        PRINTF("set_send_ui error!");
        return;
    }
    strlcpy(msg->title, "Send", msg->titleLength);
    PRINTF("msg->chainId    = %s\n", msg->network_ticker);
    cian_ui_info_t infos[4];
    infos[0] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_STETH_VAULT_ADDR, "STETH"};
    infos[1] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_RETH_VAULT_ADDR, "RETH"};
    infos[2] = (cian_ui_info_t){UNUSED, (uint8_t *) ARBITRUM_WSTETH_VAULT_ADDR, "WSTETH"};
    infos[3] = (cian_ui_info_t){UNUSED, (uint8_t *) OPTIMISM_WSTETH_VAULT_ADDR, "WSTETH"};
    for (size_t i = 0; i < sizeof(infos) / sizeof(infos[0]); i++) {
        if (memcmp((uint8_t *) msg->pluginSharedRO->txContent->destination,
                   infos[i].contract_address,
                   ADDRESS_LENGTH) == 0) {
            amountToString(context->amount_in_out,
                           sizeof(context->amount_in_out),
                           WEI_TO_ETHER,
                           infos[i].ticker,
                           msg->msg,
                           msg->msgLength);
            return;
        }
    }
}

static void set_wrapper_send_ui1(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgScreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_WRAPPER_DEPOSIT) {
        PRINTF("set_wrapper_send_ui1 error!");
        return;
    }
    strlcpy(msg->title, "Send(1/2)", msg->titleLength);
    amountToString(msg->pluginSharedRO->txContent->value.value,
                   msg->pluginSharedRO->txContent->value.length,
                   WEI_TO_ETHER,
                   "ETH",
                   msg->msg,
                   msg->msgLength);
}

static void set_wrapper_send_ui2(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_WRAPPER_DEPOSIT) {
        PRINTF("set_wrapper_send_ui2 error!");
        return;
    }
    strlcpy(msg->title, "Send(2/2)", msg->titleLength);
    amountToString(context->amount_in_out,
                   sizeof(context->amount_in_out),
                   WEI_TO_ETHER,
                   "WETH",
                   msg->msg,
                   msg->msgLength);
}

static void set_wrapper_deposit_wapto_ui(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_WRAPPER_DEPOSIT) {
        PRINTF("set_wrapper_deposit_wapto_ui error!");
        return;
    }
    strlcpy(msg->title, "SwapForDeposit", msg->titleLength);
    cian_ui_info_t infos[4];
    infos[0] =
        (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_STETH_VAULT_WRAPPER_ADDR, "STETH(min)"};
    infos[1] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_RETH_VAULT_WRAPPER_ADDR, "RETH(min)"};
    infos[2] =
        (cian_ui_info_t){UNUSED, (uint8_t *) ARBITRUM_WSTETH_VAULT_WRAPPER_ADDR, "WSTETH(min)"};
    infos[3] =
        (cian_ui_info_t){UNUSED, (uint8_t *) OPTIMISM_WSTETH_VAULT_WRAPPER_ADDR, "WSTETH(min)"};

    for (size_t i = 0; i < sizeof(infos) / sizeof(cian_ui_info_t); i++) {
        if (memcmp((uint8_t *) msg->pluginSharedRO->txContent->destination,
                   infos[i].contract_address,
                   ADDRESS_LENGTH) == 0) {
            amountToString(context->amount_received,
                           sizeof(context->amount_received),
                           WEI_TO_ETHER,
                           infos[i].ticker,
                           msg->msg,
                           msg->msgLength);
            return;
        }
    }
}

static void set_wrapper_withdraw_wapto_ui(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_WRAPPER_WITHDRAW) {
        PRINTF("set_wrapper_withdraw_wapto_ui error!");
        return;
    }
    strlcpy(msg->title, "SwapGet", msg->titleLength);
    if (context->is_get_weth) {
        amountToString(context->amount_received,
                       sizeof(context->amount_received),
                       WEI_TO_ETHER,
                       "WETH(min)",
                       msg->msg,
                       msg->msgLength);
    } else {
        amountToString(context->amount_received,
                       sizeof(context->amount_received),
                       WEI_TO_ETHER,
                       "ETH(min)",
                       msg->msg,
                       msg->msgLength);
    }
}

static void set_wrapper_send_wsteth_ui(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgScreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_WRAPPER_DEPOSIT_WSTETH) {
        PRINTF("set_wrapper_send_ui1 error!");
        return;
    }
    strlcpy(msg->title, "Send", msg->titleLength);
    amountToString(msg->pluginSharedRO->txContent->value.value,
                   msg->pluginSharedRO->txContent->value.length,
                   WEI_TO_ETHER,
                   "WSTETH",
                   msg->msg,
                   msg->msgLength);
}

static void set_withdraw_ui(ethQueryContractUI_t *msg, const context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);

    cian_ui_info_t infos[8];
    infos[0] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_STETH_VAULT_ADDR, "STETH"};
    infos[1] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_STETH_VAULT_WRAPPER_ADDR, "STETH"};
    infos[2] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_RETH_VAULT_ADDR, "RETH"};
    infos[3] = (cian_ui_info_t){UNUSED, (uint8_t *) ETHEREUM_RETH_VAULT_WRAPPER_ADDR, "RETH"};
    infos[4] = (cian_ui_info_t){UNUSED, (uint8_t *) ARBITRUM_WSTETH_VAULT_ADDR, "WSTETH"};
    infos[5] = (cian_ui_info_t){UNUSED, (uint8_t *) ARBITRUM_WSTETH_VAULT_WRAPPER_ADDR, "WSTETH"};
    infos[6] = (cian_ui_info_t){UNUSED, (uint8_t *) OPTIMISM_WSTETH_VAULT_ADDR, "WSTETH"};
    infos[7] = (cian_ui_info_t){UNUSED, (uint8_t *) OPTIMISM_WSTETH_VAULT_WRAPPER_ADDR, "WSTETH"};

    switch (context->selectorIndex) {
        case VAULT_WITHDRAW:
        case VAULT_DELEVERAGE_WITHDRAW:
        case VAULT_WRAPPER_WITHDRAW:
        case VAULT_WRAPPER_WITHDRAW_WSTETH:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            for (size_t i = 0; i < sizeof(infos) / sizeof(cian_ui_info_t); i++) {
                if (memcmp((uint8_t *) msg->pluginSharedRO->txContent->destination,
                           infos[i].contract_address,
                           ADDRESS_LENGTH) == 0) {
                    amountToString(context->amount_in_out,
                                   sizeof(context->amount_in_out),
                                   WEI_TO_ETHER,
                                   infos[i].ticker,
                                   msg->msg,
                                   msg->msgLength);
                }
            }
            break;
        default:
            PRINTF("set_withdraw_ui error!");
            break;
    }
}

static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    strlcpy(msg->title, "Beneficiary", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    if (msg->msgLength < 43) {
        PRINTF("msg length error!");
        return;
    }
    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    getEthAddressStringFromBinary(
        context->beneficiary,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

static void set_receive_token_ui(ethQueryContractUI_t *msg, context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_DELEVERAGE_WITHDRAW &&
        context->selectorIndex != VAULT_WRAPPER_WITHDRAW_WSTETH) {
        PRINTF("set_receive_token_ui error!");
        return;
    }
    strlcpy(msg->title, "Receive", msg->titleLength);
    if (context->selectorIndex == VAULT_DELEVERAGE_WITHDRAW) {
        for (size_t i = 0; i < sizeof(TOKEN_TICKER_COLLECTION) / sizeof(token_ticker_t); i++) {
            if (memcmp((uint8_t *) context->token_received,
                       TOKEN_TICKER_COLLECTION[i].token_address,
                       ADDRESS_LENGTH) == 0) {
                strlcpy(msg->msg, TOKEN_TICKER_COLLECTION[i].token_name, msg->msgLength);
                return;
            }
        }
    } else if (context->selectorIndex == VAULT_WRAPPER_WITHDRAW_WSTETH) {
        strlcpy(msg->msg, "WSTETH", msg->msgLength);
    }
}

static void set_warning_ui(ethQueryContractUI_t *msg, context_t *context) {
    PRINTF("msgscreenIndex = %d\n", context->selectorIndex);
    if (context->selectorIndex != VAULT_DELEVERAGE_WITHDRAW) {
        PRINTF("set_warning_ui error!");
        return;
    }
    strlcpy(msg->title, "Warning!", msg->titleLength);
    strlcpy(msg->msg,
            "You are using a deleveraged withdrawal and may incur some deleveraging losses!",
            msg->msgLength);
}

static screens_t get_screen(ethQueryContractUI_t *msg, context_t *context) {
    if (msg->screenIndex >= MAX_SCREEN_NUM) {
        PRINTF("get_screen error!");
        return ERROR;
    }

    typedef struct {
        uint8_t selectorIndex;
        screens_t screens[MAX_SCREEN_NUM];
    } ScreenInfo;
    const ScreenInfo screenInfos[] = {
        {VAULT_DEPOSIT, {VAULT_SEND_SCREEN, BENEFICIARY_SCREEN, ERROR, ERROR}},

        {VAULT_WITHDRAW, {VAULT_WITHDRAW_SCREEN, BENEFICIARY_SCREEN, ERROR, ERROR}},

        {VAULT_DELEVERAGE_WITHDRAW,
         {VAULT_WITHDRAW_SCREEN, VAULT_RECEIVE_TOKEN_SCREEN, WARNING_SCREEN, BENEFICIARY_SCREEN}},

        {VAULT_WRAPPER_DEPOSIT,
         {WARAPPER_SEND_SCREEN1, WARAPPER_SEND_SCREEN2, WARAPPER_DEPOSIT_SWAPTO_SCREEN, ERROR}},

        {VAULT_WRAPPER_WITHDRAW,
         {VAULT_WITHDRAW_SCREEN, WARAPPER_WITHDRAW_SWAPTO_SCREEN, BENEFICIARY_SCREEN, ERROR}},

        {VAULT_WRAPPER_DEPOSIT_WSTETH,
         {WARAPPER_DEPOSIT_WSTETH_SCREEN, BENEFICIARY_SCREEN, ERROR, ERROR}},

        {VAULT_WRAPPER_WITHDRAW_WSTETH,
         {VAULT_WITHDRAW_SCREEN, VAULT_RECEIVE_TOKEN_SCREEN, BENEFICIARY_SCREEN, ERROR}},
    };
    for (size_t i = 0; i < sizeof(screenInfos) / sizeof(ScreenInfo); i++) {
        if (context->selectorIndex == screenInfos[i].selectorIndex) {
            return screenInfos[i].screens[msg->screenIndex];
        }
    }
    return ERROR;
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;
    PRINTF("handle_query_contract_ui  %d\n", msg->screenIndex);
    screens_t screen = get_screen(msg, context);
    if (screen == ERROR) {
        PRINTF("get_screen error\n");
    }

    switch (screen) {
        case VAULT_SEND_SCREEN:
            set_send_ui(msg, context);
            break;
        case WARAPPER_SEND_SCREEN1:
            set_wrapper_send_ui1(msg, context);
            break;
        case WARAPPER_SEND_SCREEN2:
            set_wrapper_send_ui2(msg, context);
            break;
        case WARAPPER_DEPOSIT_SWAPTO_SCREEN:
            set_wrapper_deposit_wapto_ui(msg, context);
            break;
        case WARAPPER_WITHDRAW_SWAPTO_SCREEN:
            set_wrapper_withdraw_wapto_ui(msg, context);
            break;
        case VAULT_WITHDRAW_SCREEN:
            set_withdraw_ui(msg, context);
            break;
        case BENEFICIARY_SCREEN:
            set_beneficiary_ui(msg, context);
            break;
        case WARAPPER_DEPOSIT_WSTETH_SCREEN:
            set_wrapper_send_wsteth_ui(msg, context);
            break;
        case VAULT_RECEIVE_TOKEN_SCREEN:
            set_receive_token_ui(msg, context);
            break;
        case WARNING_SCREEN:
            set_warning_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
