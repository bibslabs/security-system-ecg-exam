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

@Entity('connection')
class Connection {
	@PrimaryColumn()
	id: string;

	@Column()
	socket_id: string;

	@JoinColumn({ name: 'device_id' })
	@ManyToOne(() => Device)
	device_id: Device;

	@CreateDateColumn()
	created_at: Date;

	@UpdateDateColumn()
	updated_at: Date;

	constructor() {
		if (!this.id) {
			this.id = uuid();
		}
	}
}

export { Connection };
