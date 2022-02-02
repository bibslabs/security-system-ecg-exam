import { getCustomRepository, Repository } from 'typeorm';
import { User } from '../entities/User';
import { UsersRepository } from '../repositories/UsersRepository';

interface IUserCreate {
	username: string;
	password?: string;
	first_name?: string;
	last_name?: string;
	crm?: string;
}

class UsersService {
	private usersRepository = new Repository<User>();

	constructor() {
		this.usersRepository = getCustomRepository(UsersRepository);
	}

	async create({
		username,
		password,
		first_name,
		last_name,
		crm,
	}: IUserCreate) {
		const userExist = await this.usersRepository.findOne({
			first_name,
			last_name,
		});

		if (userExist) {
			throw new Error('Já existe um paciente com esse nome salvo');
		}

		const user = this.usersRepository.create({
			username,
			password,
			first_name,
			last_name,
			crm,
		});

		await this.usersRepository.save(user);

		return user;
	}

	async list() {
		return this.usersRepository.find();
	}

	async findByUsername(username: string) {
		const listUser = await this.usersRepository.findOne({
			where: { username },
		});

		return listUser;
	}
}

export { UsersService };
