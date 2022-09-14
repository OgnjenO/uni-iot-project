const express = require('express')
const app = express()
const port = 5498
const { WebSocketServer } = require('ws');
//const wss = new WebSocket.server({ port: 5499 });
const path = require('path');
const { createServer } = require('http');

const server = createServer(app);
const wss = new WebSocketServer({ port: 5499 });

const cors = require('cors');
app.use(cors());

let client = null;

wss.on('connection', function (ws) {
  client = ws;
//  const id = setInterval(function () {
//    ws.send(JSON.stringify(process.memoryUsage()), function () {
//      //
//      // Ignore errors.
//      //
//    });
//  }, 100);
  console.log('started client interval');

  ws.on('close', function () {
    console.log('stopping client interval');
    //clearInterval(id);
  });
});

app.get('/', (req, res) => {
  res.send('WORKS!');
  console.log(new Date().toUTCString() + " WORKS!");
})

app.get('/keypad/:key', (req, res) => {
  const {
    key
  } = req.params
  console.log(new Date().toUTCString() + " Keypad: " + key);
  if(client == null) return;
  client.send(JSON.stringify({event: 'keypad', key: key}), function () {
      //
      // Ignore errors.
      //
  });
});

app.get('/joystick/:y/:x', (req, res) => {
  const {
    y,
    x
  } = req.params
  console.log(new Date().toUTCString() + " Joystick: " + x + "|" + y);
  if(client == null) return;
  client.send(JSON.stringify({ event: 'joystick', x: x, y: y}), function () {
      //
      // Ignore errors.
      //
  });
});

app.get('/rotencoder/:value', (req, res) => {
  const {
    value
  } = req.params
  console.log(new Date().toUTCString() + " Rotencoder: " + value);
  if(client == null) return;
  client.send(JSON.stringify({event: 'rotencoder', value: value}), function () {
      //
      // Ignore errors.
      //
  });
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
