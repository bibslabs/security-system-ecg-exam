import { EntityRepository, Repository } from "typeorm";
import { SocketID } from "../entities/SocketID";

@EntityRepository(SocketID)
class SocketIDsRepository extends Repository<SocketID> {}

export { SocketIDsRepository };