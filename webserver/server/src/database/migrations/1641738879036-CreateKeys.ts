import { MigrationInterface, QueryRunner, Table, Timestamp } from 'typeorm';

export class CreateKeys1641738879036 implements MigrationInterface {
	public async up(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.createTable(
			new Table({
				name: 'keys',
				columns: [
					{
						name: 'id',
						type: 'uuid',
						isPrimary: true,
					},
					{
						name: 'key',
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
		await queryRunner.dropTable('keys');
	}
}
