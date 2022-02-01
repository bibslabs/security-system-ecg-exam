import React, { useState, FormEvent } from 'react';
import { io } from 'socket.io-client';

import './styles.css';

// import createConnection from '../../services/socket';
import api from '../../services/api';

import PageHeader from '../../components/PageHeader';
import Select from '../../components/Select';

function Exam() {
	const [patientId, setPatient] = useState('');
	const [deviceId, setDevice] = useState('');
	

	const getElements = async () => {
		try {
			const response = await api.get(`/devices/${deviceId}`, {});

			console.log(response.data);
		} catch (error) {
			console.error(error);
		}

		try {
			const response = await api.get(`/patients/${patientId}`, {});

			console.log(response.data);
		} catch (error) {
			console.error(error);
		}

		// const connectionId = createConnection();
		// console.log(connectionId);
	};

	return (
		<div id="page-exam" className="container">
			<PageHeader title="Exame ECG" />

			<main id="main">
				<div className="title-container">
					<h2>Dados do exame:</h2>
				</div>

				<div className="button-container">
					<button
						id="button-connection"
						onClick={getElements}
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
