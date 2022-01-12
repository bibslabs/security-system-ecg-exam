import { Request, Response } from 'express';
import { getCustomRepository } from 'typeorm';
import { AdminsRepository } from '../repositories/AdminsRepository';

class AdminsController {
	async create(request: Request, response: Response): Promise<Response> {
		const { username, password } = request.body;

		const adminRepository = getCustomRepository(AdminsRepository);

		const admin = await adminRepository.create({
			username,
			password,
		});

		await adminRepository.save(admin);

		return response.json(admin);
	}
}

export { AdminsController };
