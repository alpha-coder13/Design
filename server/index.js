const express = require('express');
const app = express();
const {rateLimiter} =  require('./middle-wares/rate-limiter-middleware')

app.use(express.json());


app.get('/',rateLimiter,(req,res)=>{
    // console.log(req.socket.remoteAddress)
    console.log(req.url)
    res.send('hello')
})

app.listen(process.env.SERVER-PORT,'localhost',()=>{
    console.log('serveron');
})