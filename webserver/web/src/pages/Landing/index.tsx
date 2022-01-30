import React, { useState, useEffect } from "react";
import { Link } from 'react-router-dom'


import logoImg from '../../assets/images/logo-header.png';

import loginIcon from "../../assets/images/icons/fi-rr-stethoscope.svg";
import adminIcon from "../../assets/images/icons/fi-rr-following.svg";

// import api from '../../services/api';

import './styles.css';

function Landing() {
    // const [connection, setConnection] = useState(0);

    return (
       <div id="page-landing">
           <div id="page-landing-content" className="container">
               <div className="logo-container">
                   <img src={logoImg} alt="Dados Seguros" />
                   <h2>Dados seguros, saúde garantida</h2>
               </div>
               
               <div className="buttons-container">
                    <Link to="/login" className="login">
                        <img src={loginIcon} className="icon" alt="Login" />
                        Login
                    </Link> 

                    <Link to="/admin" className="admin">
                        <img src={adminIcon} className="icon" alt="Admin Login" />
                        Admin
                    </Link>
               </div>
           </div>
       </div> 
    )
}

export default Landing;