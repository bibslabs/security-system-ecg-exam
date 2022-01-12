import { Entity, Column, CreateDateColumn, PrimaryColumn } from 'typeorm';

import { v4 as uuid } from 'uuid';

@Entity('admin')
class Admin {
	@PrimaryColumn()
	id: string;

	@Column()
	username: string;

    @Column()
	password: string;

	@CreateDateColumn()
	created_at: Date;

    constructor() {
        if (!this.id){
            this.id = uuid();
        }
    }
}

export { Admin };
