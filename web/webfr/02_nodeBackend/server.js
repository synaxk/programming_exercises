const http = require('http');
const app = require('./app.js');

const port = process.env.NodePort || 3000
app.set('port', port);

const server = http.createServer(app);
server.listen(port);

