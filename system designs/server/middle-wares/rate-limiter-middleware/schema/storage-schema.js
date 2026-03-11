const getSchema = (clientObject, tokenObject)=>{
    return {
        clientID :clientObject.clientID,
        requestURL : clientObject.requestURL,
        requestTimestamp : clientObject.requestTimestamp,
        tokenBucket : tokenObject,
    }
}

module.exports = {getSchema}