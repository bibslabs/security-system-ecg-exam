import { Request, Response } from 'express'
import { KeysServices } from '../services/KeysServices'

class KeysController {
    async create(request: Request, response: Response): Promise<Response>{
        const { key } = request.body;
        
        const keysServices = new KeysServices();

        const keys = await keysServices.create(key); 

        return response.json(keys);
    }
}

export{ KeysController }