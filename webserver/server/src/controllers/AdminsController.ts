import { Request, Response } from 'express';
import { AdminsServices } from '../services/AdminsServices';

class AdminsController {
	async create(request: Request, response: Response): Promise<Response> {
		const { username, password } = request.body;

		const adminsServices = new AdminsServices();

		try {
			const admin = await adminsServices.create({
				username,
				password,
			});
	
			return response.json(admin);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
	async list(request: Request, response: Response): Promise<Response> {
		const adminsServices = new AdminsServices();

		try{
			const admins = await adminsServices.list();

			return response.json(admins);				
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
}

export { AdminsController };
