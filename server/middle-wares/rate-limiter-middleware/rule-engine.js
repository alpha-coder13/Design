const { ClientObject } = require("./client-object");

const ruleBook = {
    '_tbk' : {
        isValidRequest(object,timestampOld = Date.now(),thisArg){
            if(clientobject instanceof ClientObject){
                    thisArg = this.generateToken(object.requestTimestamp,timestampOld,thisArg);
                    if(parseInt(thisArg.TokenCount)!=='NaN'){
                        if(thisArg.tokenBucket.tokenCount - object.urlWeight < 0){
                            return [false,thisArg];
                        }else{
                            thisArg.tokenBucket.tokenCount -= object.requestWeight;
                            return [true,thisArg];
                        }
                    }else{
                        throw new TypeError('the count is NaN');
                    }
            }else{
                throw new TypeError('object passed is not of type ClientBucket');
            }
        },
        generateToken(timestampNew,timestampOld, thisArg){
            thisArg.tokenBucket.tokenCount += ((timestampNew - timestampOld)/thisArg.interval);
            thisArg.tokenBucket.tokenCount = Math.min(thisArg.tokenBucket.tokenCount,thisArg.tokenBucket.bucketSize);
            return thisArg;
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