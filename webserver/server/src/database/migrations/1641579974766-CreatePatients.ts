import { MigrationInterface, QueryRunner, Table } from 'typeorm';

export class CreatePatient1641180654705 implements MigrationInterface {
	public async up(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.createTable(
			new Table({
				name: 'patients',
				columns: [
					{
						name: 'id',
						type: 'uuid',
						isPrimary: true,
					},
					{
						name: 'first_name',
						type: 'varchar',
					},
					{
						name: 'last_name',
						type: 'varchar',
					},
					{
						name: 'height',
						type: 'decimal',
					},
					{
						name: 'weight',
						type: 'decimal',
					},
					{
						name: 'birth_date',
						type: 'date',
					},
					{
						name: 'age',
						type: 'int',
					},
					{
						name: 'email',
						type: 'varchar',
					},
					{
						name: 'sex',
						type: 'varchar',
					},
					{
						name: 'created_at',
						type: 'timestamp',
						default: 'now()',
					},
				],
			})
		);
	}

	public async down(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.dropTable('patients');
	}
}
