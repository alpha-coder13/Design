const path = require("path");
const { ClientObject } = require("./objects/client-object")
const { readFile, writeFile } = require('fs/promises');
const { fetchRule } = require("./rules/rule-engine");
const { TokenBucket } = require("./objects/token-bucket-object");
const { getSchema } = require("./schema/storage-schema");
const { GetKey } = require("./utils/key-gen");


 rateLimiter = async function (req, res, next) {
    let incomingIP = req.socket.remoteAddress;
    let incomingURL = req.url;
    let client = new ClientObject(incomingIP, incomingURL);
    let rules = fetchRule(client.rule);
    let cacheID =  new GetKey(client.clientID, client.requestURL)

}

module.exports = {rateLimiter}