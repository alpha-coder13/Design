const { ClientObject } = require("../objects/client-object");

const ruleBook = {
    '_tbk' : {
        isValidRequest(object,thisArg){
            if(object instanceof ClientObject){
                thisArg.tokenBucket = this.generateToken(object.requestTimestamp,thisArg);
                if(parseInt(thisArg.tokenBucket.tokenCount)!==NaN){
                    let currentCount = parseInt(thisArg.tokenBucket.tokenCount);
                    if(currentCount - object.requestWeight < 0){
                        return [false,thisArg];
                    }else{
                        thisArg.tokenBucket.tokenCount =  currentCount - object.requestWeight;
                        return [true,thisArg];
                    }
                }else{
                    throw new TypeError('the count is NaN');
                }
            }else{
                throw new TypeError('object passed is not of type ClientBucket');
            }
        },
        generateToken(timestampNew, thisArg){
            // console.log(thisArg);
            thisArg.tokenBucket.tokenCount = parseInt(thisArg.tokenBucket.tokenCount)+ Math.floor((timestampNew - parseInt(thisArg.requestTimestamp))/((parseInt(thisArg.tokenBucket.tokenBucketInterval))*1000));
            // console.log(thisArg.tokenBucket.tokenCount);
            thisArg.tokenBucket.tokenCount = thisArg.tokenBucket.tokenCount > parseInt(thisArg.tokenBucket.bucketSize)? parseInt(thisArg.tokenBucket.bucketSize) :thisArg.tokenBucket.tokenCount;
            return thisArg.tokenBucket;
        },
    }
}

const fetchRule = (key) => {
    if(ruleBook[key] != undefined){
        return ruleBook[key];
    }else{
        throw new ReferenceError('Provided Key is not Found in Rule Book');
    }
}

module.exports = {fetchRule};