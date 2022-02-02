import { io } from "../http";
import { ConnectionsService } from "../services/ConnectionsService";
import { UsersService } from "../services/UsersService";


io.on("connect", (socket) =>{
    const connectionsService = new ConnectionsService();
    const usersService = new UsersService();

    socket.on("client_first_access", async (params) => {
        const socket_id = socket.id;
        const {patient, username, device} = params;

        console.log(params);

        const userExists = await usersService.findByUsername(username);

        if(!userExists) {
            const user = await usersService.create(username);

            await connectionsService.create({
                socket_id,
                user_id: user.id
            })
        } else {
            await connectionsService.create({
                socket_id,
                user_id: userExists.id
            })
        }
        //Salvar conexão com o socket_is e o user_id
        
    })
})