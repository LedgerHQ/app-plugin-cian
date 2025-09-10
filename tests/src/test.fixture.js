import Zemu, { DEFAULT_START_OPTIONS, DeviceModel } from '@zondax/zemu';
import Eth from '@ledgerhq/hw-app-eth';
import { parseEther, parseUnits, RLP } from "ethers/lib/utils";
import { generate_plugin_config } from './generate_plugin_config';

const transactionUploadDelay = 60000;
const jestTimeOut = 30000
const pluginName = "cian";

async function waitForAppScreen(sim) {
    await sim.waitUntilScreenIsNot(sim.getMainMenuSnapshot(), transactionUploadDelay);
}

const sim_options_nano = {
    ...DEFAULT_START_OPTIONS,
    logging: true,
    X11: true,
    startDelay: 5000,
    startText: 'is ready'
};

const Resolve = require('path').resolve;

const NANOSP_ETH_PATH = Resolve('elfs/ethereum_nanosp.elf');
const NANOX_ETH_PATH = Resolve('elfs/ethereum_nanox.elf');

const NANOSP_PLUGIN_PATH = Resolve('elfs/plugin_nanosp.elf');
const NANOX_PLUGIN_PATH = Resolve('elfs/plugin_nanox.elf');

const nano_models: DeviceModel[] = [
    { name: 'nanosp', letter: 'SP', path: NANOSP_PLUGIN_PATH, eth_path: NANOSP_ETH_PATH },
    { name: 'nanox', letter: 'X', path: NANOX_PLUGIN_PATH, eth_path: NANOX_ETH_PATH }
];

const SPECULOS_ADDRESS = '0xFE984369CE3919AA7BB4F431082D027B4F8ED70C';
const RANDOM_ADDRESS = '0x489ee077994B6658eAfA855C308275EAd8097C4A'
const ledgerPath = "44'/60'/0'/0"

const ethereum_network = {
    name: 'ethereum',
    chainId: 1,
};
const arbitrum_network = {
    name: "arbitrum",
    chainId: 42161,
};
const optimism_network = {
    name: "optimism",
    chainId: 10,
};


let genericTx = {
    nonce: Number(0),
    gasLimit: Number(21000),
    gasPrice: parseUnits('1', 'gwei'),
    value: parseEther('1'),
    chainId: 1,
    to: RANDOM_ADDRESS,
    data: null,
};

const TIMEOUT = 1000000;

// Generates a serializedTransaction from a rawHexTransaction copy pasted from etherscan.
function txFromEtherscan(rawTx) {
    // Remove 0x prefix
    rawTx = rawTx.slice(2);

    let txType = rawTx.slice(0, 2);
    if (txType == "02" || txType == "01") {
        // Remove "02" prefix
        rawTx = rawTx.slice(2);
    } else {
        txType = "";
    }

    let decoded = RLP.decode("0x" + rawTx);
    if (txType != "") {
        decoded = decoded.slice(0, decoded.length - 3); // remove v, r, s
    } else {
        decoded[decoded.length - 1] = "0x"; // empty
        decoded[decoded.length - 2] = "0x"; // empty
        decoded[decoded.length - 3] = "0x01"; // chainID 1
    }

    // Encode back the data, drop the '0x' prefix
    let encoded = RLP.encode(decoded).slice(2);

    // Don't forget to prepend the txtype
    return txType + encoded;
}

function populateTransaction(contractAddr, inputData, chainId, value = "0.0") {
    // Get the generic transaction template
    let unsignedTx = genericTx;
    //adapt to the appropriate network
    unsignedTx.chainId = chainId;
    // Modify `to` to make it interact with the contract
    unsignedTx.to = contractAddr;
    // Modify the attached data
    unsignedTx.data = inputData;
    // Modify the number of ETH sent
    unsignedTx.value = parseEther(value);

    // Create serializedTx and remove the "0x" prefix
    return ethers.utils.serializeTransaction(unsignedTx).slice(2);
}

function zemu(device, networkName, func) {
    return async () => {
        jest.setTimeout(TIMEOUT);
        let elf_path;
        let lib_elf;
        elf_path = device.eth_path;
        lib_elf = { 'CIAN': device.path };
        const sim = new Zemu(elf_path, lib_elf);
        const networkConfig = await generate_plugin_config(networkName);
        try {
            await sim.start({ ...sim_options_nano, model: device.name });
            const transport = await sim.getTransport();
            const eth = new Eth(transport);
            eth.setLoadConfig({
                baseURL: null,
                extraPlugins: networkConfig,
            });
            await func(sim, eth);
        } finally {
            await sim.close();
        }
    };
}

module.exports = {
    zemu,
    waitForAppScreen,
    genericTx,
    nano_models,
    SPECULOS_ADDRESS,
    RANDOM_ADDRESS,
    jestTimeOut,
    pluginName,
    ledgerPath,
    txFromEtherscan,
    ethereum_network,
    arbitrum_network,
    optimism_network,
    populateTransaction,
}
