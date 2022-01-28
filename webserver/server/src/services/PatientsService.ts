import { getCustomRepository, Repository } from 'typeorm';
import { Patient } from '../entities/Patient';
import { PatientsRepository } from '../repositories/PatientsRepository';

interface IPatientCreate {
	first_name: string;
	last_name: string;
	height: number;
	weight: number;
	birth_date: Date;
	age: number;
	email: string;
	sex: string;
}

class PatientsService {
	private patientsRepository = new Repository<Patient>();

	constructor() {
		this.patientsRepository = getCustomRepository(PatientsRepository);
	}

	async create({
		first_name,
		last_name,
		height,
		weight,
		birth_date,
		age,
		email,
		sex,
	}: IPatientCreate) {
		const patientExist = await this.patientsRepository.findOne({
			first_name,
			last_name,
		});

		if (patientExist) {
			throw new Error('Já existe um paciente com esse nome salvo');
		}

		const patient = this.patientsRepository.create({
			first_name,
			last_name,
			height,
			weight,
			birth_date,
			age,
			email,
			sex,
		});

        await this.patientsRepository.save(patient);

        return patient;
	}

    async list() {
        return this.patientsRepository.find();
    }
}

export { PatientsService };
