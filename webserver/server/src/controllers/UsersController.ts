import { Request, Response } from 'express';
import { UsersService } from '../services/UsersService';

class UsersController {
	async create(request: Request, response: Response): Promise<Response> {
		const { username, password, first_name, last_name, crm } = request.body;

		const usersService = new UsersService();

		try {
			const user = await usersService.create({
				username,
				password,
				first_name,
				last_name,
				crm,
			});

			return response.json(user);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}

	async list(request: Request, response: Response): Promise<Response> {
		const usersService = new UsersService();

		try {
			const users = await usersService.list();

			return response.json(users);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}

	async showByUserID(request: Request, response: Response) {
		try {
			const { username } = request.params;

			const userServices = new UsersService();

			const user = await userServices.findByUsername(username);

			return response.json(user);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
}
export { UsersController };
