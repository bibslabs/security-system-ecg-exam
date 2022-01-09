import {
	Entity,
	Column,
	CreateDateColumn,
	PrimaryColumn,
} from 'typeorm';

import { v4 as uuid }  from 'uuid';

@Entity('devices')
class Device {

	@PrimaryColumn()
	id: string;

	@Column()
	size: string;

	@Column()
	token: string;

    @Column()
    socket_address: string;

	@Column()
    wave_key: string;
    
    @Column()
	status: number;

	@CreateDateColumn()
	created_at: Date;

    constructor(){
        if(!this.id) {
            this.id = uuid();
        }
    }
}

export { Device };
