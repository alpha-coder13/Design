const requestWeight = new Map();
requestWeight.set('url',{'weight':'1','rule':'_tbk'});

const getWeight = function(url){
    return parseInt(requestWeight.get(url).weight);
}

const getRule = function(url){
    return requestWeight.get(url).rule;
}

module.exports = {getWeight, getRule}