import { MigrationInterface, QueryRunner, Table } from 'typeorm';

export class CreateDevice1641576624493 implements MigrationInterface {
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
						name: 'size',
						type: 'varchar',
					},
					{
						name: 'token',
						type: 'varchar',
					},
					{
						name: 'mac_address',
						type: 'varchar',
					},
					{
						name: 'status',
						type: 'integer',
                        isNullable: true,
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
		await queryRunner.dropTable('devices');
	}
}
