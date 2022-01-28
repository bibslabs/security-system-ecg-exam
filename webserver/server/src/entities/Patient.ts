import { Entity, Column, CreateDateColumn, PrimaryColumn } from 'typeorm';

import { v4 as uuid } from 'uuid';

@Entity('patients')
class Patient {
	@PrimaryColumn()
	id: string;

	@Column()
	first_name: string;

	@Column()
	last_name: string;

	@Column()
	height: number;

	@Column()
	weight: number;

	@Column()
	birth_date: Date;

	@Column()
	age: number;

	@Column()
	email: string;

	@Column()
	sex: string;

	@CreateDateColumn()
	created_at: Date;

	constructor() {
		if (!this.id) {
			this.id = uuid();
		}
	}
}

export { Patient };
