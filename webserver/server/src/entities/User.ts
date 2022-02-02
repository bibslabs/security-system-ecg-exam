import {Entity, PrimaryColumn, Column, CreateDateColumn } from 'typeorm';
import { v4 as uuid } from 'uuid';

@Entity('users')
class User {
	@PrimaryColumn()
	id: string;

	@Column()
	username: string;

    @Column()
	password: string;

    @Column()
	first_name: string;

    @Column()
	last_name: string;

    @Column()
	crm: string;

	@CreateDateColumn()
	created_at: Date;

    constructor() {
        if (!this.id){
            this.id = uuid();
        }
    }
}

export { User };