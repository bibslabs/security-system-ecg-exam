import { io } from "../http";
import { ConnectionsService } from "../services/ConnectionsService";
import { UsersService } from "../services/UsersService";

// interface IParams {
//     patient: string;
//     username: string;
//     device: string;
// }

io.on("connect", (socket) =>{
    const connectionsService = new ConnectionsService();
    const usersService = new UsersService();

    socket.on("client_first_access", async (params) => {
        const socket_id = socket.id;
        const {patient, username, device} = params; //as Iparams

        console.log(params);

        const userExists = await usersService.findByUsername(username);

        if(!userExists) {
            const user = await usersService.create(username);

            await connectionsService.create({
                socket_id,
                user_id: user.id
            })
        } else {
            const connection = await connectionsService.findByUserId(userExists.id);

           if(!connection) {
               await connectionsService.create({
                   socket_id,
                   user_id: userExists.id
               })
           } else {
               connection.socket_id = socket_id;

               await connectionsService.create(connection)
           }

        }
        //Salvar conexão com o socket_is e o user_id
        
    })
})