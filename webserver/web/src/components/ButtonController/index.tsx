import React from 'react';
import { Link } from 'react-router-dom';

import './styles.css';

interface ButtonControlerProps {
	status: string;
}



const ButtonControler: React.FC<ButtonControlerProps> = (props) => {

    return (
			<div className="button-controller">
				<button
						id="button-connection"
						// onClick={getElements}
						type="button"
					>
						iniciar exame
				</button>
			</div>
    );
}

export default ButtonControler;