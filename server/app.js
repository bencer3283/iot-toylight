const express = require('express')
const bodyParser = require('body-parser')
const app = express()
const port = process.env.PORT || 4000;

app.use(bodyParser.json())

app.get('/', (req, res) => {
  res.send('Hello World!')
})

let is_light_one_on = false;
let is_light_two_on = false;

app.post('/', (req, res) => {
    console.log('request: POST ' + JSON.stringify(req.body))
    if(req.body.id == 1){
      is_light_one_on = req.body.is_on
      if(is_light_two_on){
        res.send(true)
      }
      else res.send(false)
    }
    else if(req.body.id == 2){
      is_light_two_on = req.body.is_on
      if(is_light_one_on){
        res.send(true)
      }
      else res.send(false)
    }s
})


app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})