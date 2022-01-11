import {Entity, PrimaryColumn, Column, CreateDateColumn } from 'typeorm';
import { v4 as uuid } from 'uuid';

@Entity('keys')
class Key {
	@PrimaryColumn()
	id: string;

	@Column()
	key: string;

	@CreateDateColumn()
	created_at: Date;

    constructor() {
        if (!this.id){
            this.id = uuid();
        }
    }
}

export { Key };
