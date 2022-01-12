import { getCustomRepository, Repository } from 'typeorm';
import { Admin } from '../entities/Admin';
import { AdminsRepository } from '../repositories/AdminsRepository';

interface IAdminCreate {
	username: string;
	password: string;
}

class AdminsServices {
	private adminsRepository = new Repository<Admin>();

	constructor() {
		this.adminsRepository = getCustomRepository(AdminsRepository);
	}

	async create({ username, password }: IAdminCreate) {
		const adminExist = await this.adminsRepository.findOne({
			username,
		});
		/**TODO: 
		 * Está salvando administradores com o memso username/email
		 */

		if (adminExist) {
			return adminExist;
		}

		const admin = this.adminsRepository.create({
			username,
			password,
		});

		await this.adminsRepository.save(admin);

		return admin;
	}
}

export { AdminsServices };
