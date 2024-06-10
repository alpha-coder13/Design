const path = require("path");
const { ClientObject } = require("./client-object")
const { readFile, writeFile } = require('fs/promises');
const { fetchRule } = require("./rule-engine");
const { TokenBucket } = require("./token-bucket-object");
const { getSchema } = require("./storage-schema");

const rateLimiter = async (req, res, next) => {
    let incomingIP = req.socket.remoteAddress;
    let incomingURL = req.url;
    let client = new ClientObject(incomingIP, incomingURL);
    let rules = fetchRule(client.rule);

    let data = await readFile(path.join(__dirname, './clients.json'), 'utf-8')
    // .then(data => {
    //     console.log(JSON.parse(data));
    // }).catch(err => {
    //     console.log('Error in reading file');
    // })

    data = JSON.parse(data);

    {
        if (data instanceof Array) {
            let index = data.map((value) => value.clientID == client.clientID).indexOf(true);

            if (index !== -1) {
                let exsistingClient = data.splice(index, 1);
                let bucket = exsistingClient.tokenBucket;
                let Rules = fetchRule(client.rule);
                let [isvalid, newbucket] = Rules.isValidRequest(client, exsistingClient.requestTimestamp,exsistingClient );
                exsistingClient = newbucket;
                exsistingClient.timestamp = client.clientID;
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
                let TokenBucket =  new TokenBucket();
                data.push(getSchema(client,TokenBucket));
                data = JSON.stringify(data);
                await writeFile(path.join(__dirname, './clients.json'),data,'utf-8')
                next();
            }

        } else {
            throw new TypeError('the database is not of type array')
        }
    }// for local testing
    // console.log(JSON.parse(data));
}

