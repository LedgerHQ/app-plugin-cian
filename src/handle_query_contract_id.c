#include "cian_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    /*
        Because we need to use the pointer value of the Ethereum address in memory,
        we can't define it like this:
        cian_ui_info_t selectors[SELECTOR_UI_NUM] = {
         {VAULT_DEPOSIT, (uint8_t *) ETHEREUM_STETH_VAULT_ADDR, "STETH Deposit"},
         {VAULT_DEPOSIT, (uint8_t *) ETHEREUM_RETH_VAULT_ADDR, "RETH Deposit"},
         ...
         };
        The following definition uses the function signature and Ethereum address to
        determine the content that should be displayed on the first page when the device
        is called. If you need to add a display item, you need to modify the value of
        SELECTOR_UI_NUM and add a definition here.
    */
    cian_ui_info_t selectors[SELECTOR_UI_NUM];
    selectors[0] =
        (cian_ui_info_t){VAULT_DEPOSIT, (uint8_t *) ETHEREUM_STETH_VAULT_ADDR, "STETH Deposit"};
    selectors[1] =
        (cian_ui_info_t){VAULT_DEPOSIT, (uint8_t *) ETHEREUM_RETH_VAULT_ADDR, "RETH Deposit"};
    selectors[2] =
        (cian_ui_info_t){VAULT_DEPOSIT, (uint8_t *) ARBITRUM_WSTETH_VAULT_ADDR, "WSTETH Deposit"};
    selectors[3] =
        (cian_ui_info_t){VAULT_DEPOSIT, (uint8_t *) OPTIMISM_WSTETH_VAULT_ADDR, "WSTETH Deposit"};
    selectors[4] =
        (cian_ui_info_t){VAULT_WITHDRAW, (uint8_t *) ETHEREUM_STETH_VAULT_ADDR, "STETH Withdraw"};
    selectors[5] =
        (cian_ui_info_t){VAULT_WITHDRAW, (uint8_t *) ETHEREUM_RETH_VAULT_ADDR, "RETH Withdraw"};
    selectors[6] =
        (cian_ui_info_t){VAULT_WITHDRAW, (uint8_t *) ARBITRUM_WSTETH_VAULT_ADDR, "WSTETH Withdraw"};
    selectors[7] =
        (cian_ui_info_t){VAULT_WITHDRAW, (uint8_t *) OPTIMISM_WSTETH_VAULT_ADDR, "WSTETH Withdraw"};
    selectors[8] = (cian_ui_info_t){VAULT_DELEVERAGE_WITHDRAW, NULL, "Withdraw"};
    selectors[9] = (cian_ui_info_t){VAULT_WRAPPER_DEPOSIT, NULL, "ETH Deposit"};
    selectors[10] = (cian_ui_info_t){VAULT_WRAPPER_WITHDRAW, NULL, "Withdraw"};
    selectors[11] = (cian_ui_info_t){VAULT_WRAPPER_DEPOSIT_WSTETH, NULL, "WSTETH Deposit"};
    selectors[12] = (cian_ui_info_t){VAULT_WRAPPER_WITHDRAW_WSTETH, NULL, "WSTETH Withdraw"};

    const context_t *context = (const context_t *) msg->pluginContext;
    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);
    PRINTF("selectors length = %d\n", sizeof(selectors) / sizeof(cian_ui_info_t));
    for (uint32_t i = 0; i < sizeof(selectors) / sizeof(selectors[0]); i++) {
        PRINTF("loop selectors[%d] contract_address is: %08x \n", i, selectors[i].contract_address);
        if (context->selectorIndex == selectors[i].selectorIndex) {
            if (selectors[i].contract_address == NULL) {
                strlcpy(msg->version, selectors[i].ticker, msg->versionLength);
                msg->result = ETH_PLUGIN_RESULT_OK;
                return;
            } else if (memcmp(selectors[i].contract_address,
                              msg->txContent->destination,
                              ADDRESS_LENGTH) == 0) {
                strlcpy(msg->version, selectors[i].ticker, msg->versionLength);
                msg->result = ETH_PLUGIN_RESULT_OK;
                return;
            }
        }
    }
    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
    msg->result = ETH_PLUGIN_RESULT_ERROR;
}
