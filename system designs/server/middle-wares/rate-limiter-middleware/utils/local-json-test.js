
let data = await readFile(path.join(__dirname, '../json-storage/clients.json'), 'utf-8')

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
            await writeFile(path.join(__dirname, './json-storage/clients.json'),data,'utf-8')
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