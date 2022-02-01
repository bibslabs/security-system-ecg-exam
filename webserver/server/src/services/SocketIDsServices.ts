import { getCustomRepository } from 'typeorm';
import { SocketIDsRepository } from '../repositories/SocketIDsRepository';

class SocketIDsServices {
	async create(socket_id: string) {
		const socket_idsRepository = getCustomRepository(SocketIDsRepository);
        //verificar se a chave existe
		const socket_idExists = await socket_idsRepository.findOne({
			socket_id,
		});

        // se a chave existir retornar a chave
		if (socket_idExists) {
			return socket_idExists;
		}

		const socket_ids = socket_idsRepository.create({
			socket_id,
		});

		await socket_idsRepository.save(socket_ids);

        //senão existir, salva a chave no banco
		return socket_ids;
	}
}

export { SocketIDsServices };