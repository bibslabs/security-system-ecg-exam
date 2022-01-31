import { Request, Response } from 'express';
import { PatientsService } from '../services/PatientsService';

class PatientsController {
	async create(request: Request, response: Response): Promise<Response> {
		const {
			first_name,
			last_name,
			height,
			weight,
			birth_date,
			age,
			email,
			sex,
		} = request.body;

		const patientsService = new PatientsService();

		try {
			const patient = await patientsService.create({
				first_name,
				last_name,
				height,
				weight,
				birth_date,
				age,
				email,
				sex,
			});

			return response.json(patient);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}

	async list(request: Request, response: Response): Promise<Response> {
		const patientsService = new PatientsService();

		try {
			const patients = await patientsService.list();

			return response.json(patients);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}

	async showByPatientID(request: Request, response: Response) {
		try {
			const { id } = request.params;

			const patientServices = new PatientsService();

			const patient = await patientServices.listByPatientID(id);

			return response.json(patient);
		} catch (err) {
			return response.status(400).json({
				message: err.message,
			});
		}
	}
}
export { PatientsController };
