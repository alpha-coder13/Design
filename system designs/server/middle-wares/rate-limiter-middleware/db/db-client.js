const {createClient} = require('redis');
const dbClient  = createClient({
    socket:{
        port:process.env.REDIS-PORT,
        reconnectStrategy: (retries) => {
            if (retries > 20) {
                console.log("Too many attempts to reconnect. Redis connection was terminated");
                return new Error("Too many retries.");
            } else {
                return retries * 100; // client tries to connect with a interval of retries *500 milliseconds once the number of retries passes a threshold of 20
            } 
        }
    }
});

client.on('error' , () => {
    console.log('DB error')
})


module.exports = {dbClient}

