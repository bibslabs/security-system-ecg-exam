import express, { json, request, response } from "express";
import { createServer } from "http";
import { Server, Socket } from "socket.io";
import cors from 'cors';

import "./database";
import { routes } from "./routes";

const app = express(); 

const http = createServer(app); //criando protocolo http
const io = new Server(http); //criando protocolo websocket
app.use(cors());

io.on("connection", (socket: Socket)=> {
    console.log("conectado via websocket", socket.id);
})

// app.get("/", (request, response) => {
//     return response.json({
//         message: "olá mundo"
//     });
// });

// app.post("/", (request, response) => {
//     return response.json({
//         message: "Bom dia"
//     })
// })

app.use(express.json());

app.use(routes);

http.listen(3333, ()  => console.log("O servidor está rodando na porta 3333"));


