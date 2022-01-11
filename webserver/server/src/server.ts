import express, { json, request, response } from "express";
import "./database";
import { routes } from "./routes";

const app = express(); 

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

app.listen(3333, ()  => console.log("O servidor está rodando na porta 3333"));


