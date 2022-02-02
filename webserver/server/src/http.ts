import express, { json, request, response } from 'express';
import { createServer } from 'http';
import { Server, Socket } from 'socket.io';
import cors from 'cors';

import './database';
import { routes } from './routes';

const app = express();

const http = createServer(app); //criando protocolo http
// const io = new Server(http); //criando protocolo websocket

const io = require('socket.io')(http, {
	cors: {
		origin: 'http://localhost:3000',
		methods: ['GET', 'POST'],
		allowedHeaders: ['my-custom-header'],
		credentials: true,
	},
});

app.use(cors());
// TODO: ver como fazer esse put para atualizar o dado da tabela sempre que atualizar o socket id

// const socket_client = [];
io.on('connection', (socket: Socket) => {
	// socket_client.push({
	// 	user_id: socket.id,
	// });
	console.log('socket criado no http', socket.id);
	// console.log('socket_client', socket_client);
	socket.emit('message', 'hello14');

	socket.on('hello', (arg) => {
		console.log(arg); // world
	});

	socket.on('client-socket', (id) => {
		console.log('id do socket client', id);
	});

	var argument = 'oiii teste';
	socket.on('esp-message', (arg) => {
		console.log('arg:', arg); // message esp
		socket.broadcast.emit('client-message', arg);
		// socket_client.forEach(function (item) {
		// });
	});

	

	// socket.disconnect();
});

app.use(express.json());

app.use(routes);

export { http, io };
