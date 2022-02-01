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

io.on('connection', (socket: Socket) => {
	// app.put(`/socket/ea9398ce-e2b3-41bd-94e0-3cfc67a2791f/socket_id/${socket.id}`)
	console.log('socket criado no http', socket.id);
	socket.emit('message', 'hello14');

	socket.on('hello', (arg) => {
		console.log(arg); // world
	});

	socket.on('message-esp', (arg) => {
		console.log(arg); // world
		socket.emit('message-client', arg);
	});

	// socket.disconnect();
});

app.use(express.json());

app.use(routes);

export { http, io };
