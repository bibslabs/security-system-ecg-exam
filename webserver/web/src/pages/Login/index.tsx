import React from 'react';
import PageHeader from '../../components/PageHeader';

import './styles.css';

function Login() {
	return (
		<div id="page-login" className="container">
			<PageHeader title="Faça seu login" />
			<main id="main">
				<div className="title-container">
					<h3>Entre com as suas credenciais</h3>
				</div>
			</main>
		</div>
	);
}

export default Login;
