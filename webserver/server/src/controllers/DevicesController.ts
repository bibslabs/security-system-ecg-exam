import { Request, Response } from 'express';
import { getCustomRepository } from 'typeorm';
import { DevicesRepository } from '../repositories/DevicesRepository'

class DevicesController {
    async create(request: Request, response: Response) {
        const { token, size, mac_address } = request.body;
        const deviceRepository = getCustomRepository(DevicesRepository);
    
        const device = deviceRepository.create({
            token,
            size,
            mac_address
        })
    
        await deviceRepository.save(device);
    
        return response.json(device);
    }

}

export { DevicesController }