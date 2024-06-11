const path = require("path");
const { ClientObject } = require("./objects/client-object")
const { readFile, writeFile } = require('fs/promises');
const { fetchRule } = require("./rules/rule-engine");
const { TokenBucket } = require("./objects/token-bucket-object");
const { getSchema } = require("./schema/storage-schema");

 rateLimiter = async function (req, res, next) {
    let incomingIP = req.socket.remoteAddress;
    console.log(incomingIP);
    let incomingURL = req.url;
    let client = new ClientObject(incomingIP, incomingURL);
    let rules = fetchRule(client.rule);

    let data = await readFile(path.join(__dirname, './json-storage/clients.json'), 'utf-8')

    data = JSON.parse(data);

    {
        if (data instanceof Array) {
            let index = data.map((value) => value.clientID == client.clientID).indexOf(true);

            if (index !== -1) {
                let exsistingClient = data.splice(index, 1)[0];
                let bucket = exsistingClient.tokenBucket;
                let Rules = fetchRule(client.rule);

                let [isvalid, newbucket] = Rules.isValidRequest(client,exsistingClient );

                exsistingClient = newbucket;
                exsistingClient.requestTimestamp = client.requestTimestamp;
                data.push(exsistingClient);
                data = JSON.stringify(data);
                await writeFile(path.join(__dirname, './clients.json'),data,'utf-8')
                if(isvalid){
                    next();
                }else{
                    res.status(429).send('To Many Requests, Please try After Some time')
                }
            }
            else{
                let tokenBucket =  new TokenBucket();
                data.push(getSchema(client,tokenBucket));
                data = JSON.stringify(data);
                await writeFile(path.join(__dirname, './clients.json'),data,'utf-8')
                next();
            }

        } else {
            throw new TypeError('the database is not of type array')
        }
    }// for local testing
}

module.exports = {rateLimiter}