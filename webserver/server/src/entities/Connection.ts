import {
	Entity,
	PrimaryColumn,
	CreateDateColumn,
	Column,
	ManyToOne,
	JoinColumn,
	UpdateDateColumn,
} from 'typeorm';

import { v4 as uuid } from 'uuid';
import { Device } from './Device';
import { User } from './User';

@Entity('myconnections')
class Connection {
	@PrimaryColumn()
	id: string;

	@Column()
	socket_id: string;

	@JoinColumn({ name: 'device_id' })
	@ManyToOne(() => Device)
	device: Device;

	@Column()
    device_id: string;

	@JoinColumn({ name: 'user_id' })
	@ManyToOne(() => User)
	user: Device;

    @Column()
    user_id: string;

	@CreateDateColumn()
	created_at: Date;

	@UpdateDateColumn()
	update_at: Date;

	constructor() {
		if (!this.id) {
			this.id = uuid();
		}
	}
}

export { Connection };
