import { Request, Response } from 'express';
import { ConnectionsService } from '../services/ConnectionsService';

class ConnectionsController {
	async create(request: Request, response: Response): Promise<Response> {
		const { socket_id, device_id, user_id, id } = request.body;

		const connectionsServices = new ConnectionsService();

		try {
			const connection = await connectionsServices.create({
				socket_id,
				device_id,
				user_id,
				id
			});
	
			return response.json(connection);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
	async list(request: Request, response: Response): Promise<Response> {
		const connectionsServices = new ConnectionsService();

		try{
			const connections = await connectionsServices.list();

			return response.json(connections);				
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
}

export { ConnectionsController };