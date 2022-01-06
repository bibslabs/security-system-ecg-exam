import { MigrationInterface, QueryRunner, Table } from 'typeorm';

export class CreateDevice1641179774991 implements MigrationInterface {
	public async up(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.createTable(
			new Table({
				name: 'devices',
				columns: [
					{
						name: 'id',
						type: 'uuid',
						isPrimary: true,
					},
					{
						name: 'token',
						type: 'varchar',
					},
					{
						name: 'wave_key',
						type: 'varchar',
					},
					{
						name: 'size',
						type: 'varchar',
					},
					{
						name: 'status',
						type: 'integer',
					},
					{
						name: 'create_at',
						type: 'timestamp',
						default: 'now()',
					},
				],
			})
		);
	}

	public async down(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.dropTable('devices');
	}
}
