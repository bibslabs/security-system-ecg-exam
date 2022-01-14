import {MigrationInterface, QueryRunner, Table} from "typeorm";

export class CreateExams1642036247410 implements MigrationInterface {

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.createTable(
            new Table({
                name: 'exams',
                columns: [
                    {
                        name: 'id',
                        type: 'uuid',
                        isPrimary: true
                    },
                    {
                        name: 'patient_id',
                        type: 'uuid'
                    },
                    {
                        name: 'user_id',
                        type: 'uuid'
                    },
                    {
                        name: 'device_id',
                        type: 'uuid'
                    },
                    {
                        name: 'ecg_data',
                        type: 'varchar'
                    },
                    {
                        name: 'key',
                        type: 'uuid'
                    },
                    {
                        name: 'observation',
                        type: 'varchar'
                    },
                    {
                        name: 'duration',
                        type: 'varchar'
                    },
                    {
                        name: 'report',
                        type: 'varchar'
                    },
                    {
						name: 'created_at',
						type: 'timestamp',
						default: 'now()',
					},
                ]
            })
        )
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.dropTable('exams');
    }

}
