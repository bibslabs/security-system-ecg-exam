import {Entity, PrimaryColumn, Column, CreateDateColumn } from 'typeorm';
import { v4 as uuid } from 'uuid';

@Entity('sockets')
class SocketID {
	@PrimaryColumn()
	id: string;

	@Column()
	socket_id: string;

	@CreateDateColumn()
	created_at: Date;

    constructor() {
        if (!this.id){
            this.id = uuid();
        }
    }
}

export { SocketID };