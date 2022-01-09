import {
	MigrationInterface,
	QueryRunner,
	Table,
	TableForeignKey,
} from 'typeorm';

export class CreateConnections1641740575993 implements MigrationInterface {
	public async up(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.createTable(
			new Table({
				name: 'connections',
				columns: [
					{
						name: 'id',
						type: 'uuid',
						isPrimary: true,
					},
					{
						name: 'socket_id',
						type: 'varchar',
					},
					{
						name: 'device_id',
						type: 'uuid',
					},
					{
						name: 'created_at',
						type: 'timestamp',
						default: 'now()',
					},
					{
						name: 'update_at',
						type: 'timestamp',
						default: 'now()',
					},
				],
			})
		);

		await queryRunner.createForeignKey(
			'connections',
			new TableForeignKey({
				name: 'FKConnectionDevice',
				referencedTableName: 'devices',
				referencedColumnNames: ['id'],
				columnNames: ['device_id'],
				onDelete: 'SET NULL',
				onUpdate: 'SET NULL',
			})
		);
	}

	public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.dropForeignKey('connections', 'FKConnectionDevice');
		await queryRunner.dropTable('connections');
    }
}
