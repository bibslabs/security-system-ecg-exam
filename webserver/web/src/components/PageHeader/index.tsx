import React from 'react';
import { Link } from 'react-router-dom';

import './styles.css';

import logoImg from '../../assets/images/logo-header.png';
import backIcon from '../../assets/images/icons/fi-rr-arrow-left.svg';

function PageHeader() {
    return (
			<header className="page-header">
				<div className="top-bar-container">
					<Link to="/">
						<img src={backIcon} className="icon" alt="Voltar" />
					</Link>
					<img src={logoImg} alt="Logo sinal cardiaco" />
				</div>

				<div className="header-content">
					<strong>Exame ECG</strong>
				</div>
			</header>
    );
}

export default PageHeader;