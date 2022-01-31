import { getCustomRepository, Repository } from 'typeorm';
import { Device } from '../entities/Device';
import { DevicesRepository } from '../repositories/DevicesRepository';

interface IDeviceCreate {
	size: string;
	token: string;
	mac_address: string;
}

class DevicesService {
	private devicesRepository = new Repository<Device>();

	constructor() {
		this.devicesRepository = getCustomRepository(DevicesRepository);
	}

	async create({ size, token, mac_address }: IDeviceCreate) {
		const device = this.devicesRepository.create({
			size,
			token,
			mac_address,
		});

		await this.devicesRepository.save(device);

		return device;
	}

	async list() {
		const devicesList = this.devicesRepository.find();

		return devicesList;
	}

	async listByDeviceID(id:string) {
		const listDevice = await this.devicesRepository.find({
			where: { id },
		});

		return listDevice;
	}
}

export { DevicesService };
