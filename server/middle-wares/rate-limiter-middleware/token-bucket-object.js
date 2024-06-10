
const TokenBucket = function(tokenCount=10,bucketSize=10,tokenBucketInterval=1){
    this.tokenCount = tokenCount;
    this.bucketSize = bucketSize;
    this.tokenBucketInterval = tokenBucketInterval;
}

module.exports = {TokenBucket}