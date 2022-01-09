import { Entity, EntityRepository, Repository } from 'typeorm';

import { Device } from '../entities/Device';

@EntityRepository(Device)
class DevicesRepository extends Repository<Device> {}

export { DevicesRepository };
