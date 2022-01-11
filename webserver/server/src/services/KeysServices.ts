import { getCustomRepository } from 'typeorm';
import { KeysRepository } from '../repositories/KeysRepository';

class KeysServices {
	async create(key: string) {
		const keysRepository = getCustomRepository(KeysRepository);
        //verificar se a chave existe
		const keyExists = await keysRepository.findOne({
			key,
		});

        // se a chave existir retornar a chave
		if (keyExists) {
			return keyExists;
		}

		const keys = keysRepository.create({
			key,
		});

		await keysRepository.save(keys);

        //senão existir, salva a chave no banco
		return keys;
	}
}

export { KeysServices };
