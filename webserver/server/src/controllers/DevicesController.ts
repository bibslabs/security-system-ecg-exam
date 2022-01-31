import { Request, Response } from 'express';
import { getCustomRepository } from 'typeorm';
import { DevicesRepository } from '../repositories/DevicesRepository';
import { DevicesService } from '../services/DevicesService';

class DevicesController {
	async create(request: Request, response: Response) {
		const { token, size, mac_address } = request.body;

		const deviceRepository = getCustomRepository(DevicesRepository);

		const device = deviceRepository.create({
			token,
			size,
			mac_address,
		});

		await deviceRepository.save(device);

		return response.json(device);
	}
	async list(request: Request, response: Response): Promise<Response> {
		const devicesServices = new DevicesService();

		try {
			const devices = await devicesServices.list();

			return response.json(devices);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}

	async showByDeviceID(request: Request, response: Response) {
		try {
			const { id } = request.params;

			const devicesServices = new DevicesService();

			const device = await devicesServices.listByDeviceID(id);

			return response.json(device);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
}

export { DevicesController };
