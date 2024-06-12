const { getWeight, getRule } = require("../rules/request-weight-fetcher");

const ClientObject = function (clientIP,incomingURL) {
    this.clientID = clientIP;
    this.requestTimestamp = Date.now();
    this.requestWeight = getWeight(incomingURL);
    this.rule = getRule(incomingURL);
}

// console.log(new ClientObject('123'))

module.exports = { ClientObject }