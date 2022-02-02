import {MigrationInterface, QueryRunner, Table, TableForeignKey} from "typeorm";

export class CreateConnections1643760172484 implements MigrationInterface {

    public async up(queryRunner: QueryRunner): Promise<void> {
		await queryRunner.createTable(
			new Table({
				name: 'myconnections',
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
                        isNullable: true
					},
                    {
						name: 'user_id',
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
			'myconnections',
			new TableForeignKey({
				name: 'FKConnectionDevice',
				referencedTableName: 'devices',
				referencedColumnNames: ['id'],
				columnNames: ['device_id'],
				onDelete: 'CASCADE',
				onUpdate: 'SET NULL',
			})
		);

        await queryRunner.createForeignKey(
			'myconnections',
			new TableForeignKey({
				name: 'FKConnectionUser',
				referencedTableName: 'users',
				referencedColumnNames: ['id'],
				columnNames: ['user_id'],
				onDelete: 'CASCADE',
				onUpdate: 'SET NULL',
			})
		);
	}

	public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.dropForeignKey('connections', 'FKConnectionDevice');
        await queryRunner.dropForeignKey('connections', 'FKConnectionUser');
		await queryRunner.dropTable('connections');
    }
}
