import { EntityRepository, Repository } from "typeorm";
import { Key } from "../entities/Key";

@EntityRepository(Key)
class KeysRepository extends Repository<Key> {}

export { KeysRepository };