const getSchema = (clientObject, tokenObject)=>{
    return {
        clientID :clientObject.clientID,
        requestTimestamp : clientObject.requestTimestamp,
        tokenBucket : tokenObject,
    }
}

module.exports = {getSchema}