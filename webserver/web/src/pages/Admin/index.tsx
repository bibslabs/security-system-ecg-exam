import React, { useState, FormEvent } from 'react';
import { io } from 'socket.io-client';

import './styles.css';

// import socket from '../../services/socket';
import api from '../../services/api';

import PageHeader from '../../components/PageHeader';
import Select from '../../components/Select';
import { Link } from 'react-router-dom';

function Admin() {
	const [patientId, setPatient] = useState('');
	const [deviceId, setDevice] = useState('');

	const getElements = async () => {
		try {
			const device = await api.get(`/devices/${deviceId}`, {});

			console.log(device.data);
		} catch (error) {
			console.error(error);
		}

		try {
			const patient = await api.get(`/patients/${patientId}`, {});

			console.log(patient.data);
		} catch (error) {
			console.error(error);
		}

		// socket.on("connect", () => {
		// 	socket.emit("client_first_access")
		// })
		// console.log(connectionId)
	};

	const connection = async () => {
		const socket = io('http://localhost:3333');
		console.log('conectado', socket);
		socket.on('connect', () => {
			// either with send()
		});

		socket.emit("hello", "world");

		// handle the event sent with socket.send()
		socket.on('message', (data) => {
			console.log(data);
		});

		socket.on('message-client', (data) => {
			console.log(data);
		});

		// handle the event sent with socket.emit()
		socket.on('greetings', (elem1, elem2, elem3) => {
			console.log(elem1, elem2, elem3);
		});
	};

	async function getDevice(e: FormEvent) {
		e.preventDefault();

		const response = await api.get('/devices', {});

		getDevice(response.data);

		console.log(response);
	}

	return (
		<div id="page-admin" className="container">
			<PageHeader title="Exame ECG" />

			<main id="main">
				<div className="title-container">
					<h2>Selecione os dados do paciente</h2>
				</div>

				<div className="select-container">
					<form className="data-exam" onSubmit={getDevice}>
						<Select
							name="patientId"
							label="Nome do Paciente"
							value={patientId}
							onChange={(e) => {
								setPatient(e.target.value);
							}}
							options={[
								{
									value: '30e0c587-b14c-48e0-8007-c302ba3a835a',
									label: 'Agustín Madrigal',
								},
								{
									value: '09ca1607-6be1-45f4-97d2-edf6dfb9b537',
									label: 'Alma Madrigal',
								},
								{
									value: 'd38080a2-1f4b-4519-abd5-3f930669efe7',
									label: 'Antônio Madrigal',
								},
								{
									value: '44a207da-ba93-41ad-aec5-dd92fe6359b3',
									label: 'Bianca Espindola',
								},
								{
									value: 'de58fed5-d38a-4e2d-9a71-5447e11c4605',
									label: 'Bruno Madrigal',
								},
								{
									value: '88b56188-ade7-4920-8f00-b5d6431a9045',
									label: 'Camilo Madrigal',
								},
								{
									value: 'c6918d17-ce09-4a5b-b3b7-f604f3398b28',
									label: 'Dolores Madrigal',
								},
								{
									value: 'e59689ce-c887-4e10-9a99-a2ade931dfd7',
									label: 'Félix Madrigal',
								},
								{
									value: '272f0860-f141-4fa6-94f7-77b96e2f22ab',
									label: 'Isabela Madrigal',
								},
								{
									value: '22bcd330-84aa-423c-b8eb-c18218fdef50',
									label: 'Julieta Madrigal',
								},
								{
									value: '6a80c14b-71e9-4086-b239-b4904a469669',
									label: 'Luiza Madrigal',
								},
								{
									value: '6038594f-4508-4fee-8261-05c0d8d13c8f',
									label: 'Mirabel Madrigal',
								},
								{
									value: 'd64cc2b3-d229-442d-b12b-cb6a1bf28253',
									label: 'Pepa Madrigal',
								},
							]}
						/>
						<Select
							name="deviceId"
							label="Dispositivo Usado"
							value={deviceId}
							onChange={(e) => {
								setDevice(e.target.value);
							}}
							options={[
								{
									value: 'cb6e2737-32c7-40e2-aa22-82c62a3107a4',
									label: 'Tamanho P',
								},
								{
									value: 'ab2b4492-c089-4a4f-a532-a6f725f9eb83',
									label: 'Tamanho M',
								},
								{
									value: '2be5eaef-0857-4892-b184-bdb31a38edf0',
									label: 'Tamanho G',
								},
							]}
						/>
					</form>
				</div>

				<div className="button-container">
					<button
						id="button-connection"
						onClick={getElements}
						type="button"
					>
						iniciar exame
					</button>
					<button
						id="button-connection"
						onClick={connection}
						type="button"
					>
						iniciar conexão
					</button>

					<p></p>
				</div>
			</main>
		</div>
	);
}

export default Admin;

// const Admin: React.FC = () => {
//     const navigate = useNavigate();

//     const handleSubmit = async (event: FormEvent) => {
//         event.preventDefault();

//         try {
//             await api.post('/admin', {
//                 username
//             })

//             push('/admin');
//         } catch (error) {
//             console.error(error);
//         }
//     }

//     return (
//         <div id="page-admin">
//             <h1>Rota do admin</h1>
//             <button type="button" onClick={handleSubmit}>iniciar conexão</button>
//         </div>

//     )
