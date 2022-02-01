import React from "react";
import { io } from "socket.io-client";

// function startSocket() {

// }

const createConnection = async () => {
    try {
        const socket = io('http://localhost:3333');
        console.log(socket);

        // const response =  await api.get('/admin',{})
        // socket.on("message", function(msg){
        //     console.log("socket esta funcionando no front:", msg)
        // })
        // console.log(response);
    } catch (error) {
        console.error(error);
    }
};

export default createConnection; 