const {createHash} = require('node:crypto');

const GetKey = (key1, key2) =>{
    let hash = createHash('256');
    this.key = hash.update(`${key1}-${key2}`,'utf-8').digest('utf-8');
    return this
}

module.exports = { GetKey }