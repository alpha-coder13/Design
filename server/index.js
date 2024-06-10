const express = require('express');
const app = express();

app.use(express.json());

app.get('*',(req,res)=>{
    // console.log(req.socket.remoteAddress)
    console.log(req.url)
    res.send('hello')
})

app.listen(3000,'localhost',()=>{
    console.log('serveron');
})