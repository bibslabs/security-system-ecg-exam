import { Request, Response } from 'express'
import { SocketIDsServices } from '../services/SocketIDsServices'

class SocketIDsController {
    async create(request: Request, response: Response): Promise<Response>{
        const { socket_id } = request.body;
        
        const socket_idsServices = new SocketIDsServices();

        const socket_ids = await socket_idsServices.create(socket_id); 

        return response.json(socket_ids);
    }
}

export{ SocketIDsController }