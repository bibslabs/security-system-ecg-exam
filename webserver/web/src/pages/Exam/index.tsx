import React, { useState, FormEvent } from 'react';
import { io } from 'socket.io-client';

import './styles.css';

import api from '../../services/api';

import PageHeader from '../../components/PageHeader';

// let socket_id = '';

const patient = 'Perikitas';
const device = 'P'
const username = 'drdonatella@gmail.com';

function Exam() {
	const params = {
		patient,
		device,
		username,
	};
	const connection = async () => {
		const socket = io('http://localhost:3333');
		console.log('conectado', socket);
		socket.on('connect', () => {
			socket.emit(
				'client_first_access',
				params,
				(call: string, err: string) => {
					if (err) {
						console.error(err);
					} else {
						console.log(call);
					}
				}
			);
		});

		socket.emit('hello', 'world');

		// handle the event sent with socket.send()
		socket.on('message', (data) => {
			console.log(data);
		});

		// handle the event sent with socket.emit()
		socket.on('greetings', (elem1, elem2, elem3) => {
			console.log(elem1, elem2, elem3);
		});
	};

	// const connectionId = createConnection();
	// console.log(connectionId);

	return (
		<div id="page-exam" className="container">
			<PageHeader title="Exame ECG" />

			<main id="main">
				<div className="title-container">
					<h2>Dados do exame:</h2>
				</div>

				<div className="exam-container"></div>

				<div className="button-container">
					<button
						id="button-connection"
						onClick={connection}
						type="button"
					>
						iniciar exame
					</button>
					<p></p>
				</div>
			</main>
		</div>
	);
}

export default Exam;
