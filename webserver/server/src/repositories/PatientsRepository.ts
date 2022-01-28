import { EntityRepository, Repository } from 'typeorm';
import { Patient } from '../entities/Patient';

@EntityRepository(Patient)
class PatientsRepository extends Repository<Patient> {}

export { PatientsRepository };