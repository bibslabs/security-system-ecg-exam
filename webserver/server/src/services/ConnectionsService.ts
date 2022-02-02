import { getCustomRepository, Repository } from "typeorm";
import { Connection } from "../entities/Connection";
import { ConnectionsRepository } from "../repositories/ConnectionsRepository"

interface IConnectionCreate {
    socket_id: string;
    device_id?: string;
    user_id: string;
    id?: string;
}

class ConnectionsService {
    private connectionsRepository = new Repository<Connection>();

    constructor() {
        this.connectionsRepository = getCustomRepository(ConnectionsRepository);
    }

    async create({socket_id, device_id, user_id, id}: IConnectionCreate){
        const connection = this.connectionsRepository.create({
            socket_id,
            device_id,
            user_id,
            id,
        });

        await this.connectionsRepository.save(connection);

        return connection;
    }

    async list() {
		return this.connectionsRepository.find();
	}
}

export { ConnectionsService }