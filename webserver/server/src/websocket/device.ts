import { wss } from "../http";

wss.on("connect", (socket) =>{
    socket.on("device_first_access", params => {
        console.log(params);
    })
})