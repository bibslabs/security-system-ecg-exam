import React from "react";
import { Link } from 'react-router-dom'


import logoImg from '../../assets/images/logo-header.png'

import loginIcon from "../../assets/images/icons/fi-rr-stethoscope.svg"
import adminIcon from "../../assets/images/icons/fi-rr-following.svg"

import './styles.css'

function Landing() {
    return (
       <div id="page-landing">
           <div id="page-landing-content" className="container">
               <div className="logo-container">
                   <img src={logoImg} alt="Dados Seguros" />
                   <h2>Dados seguros, saúde garantida</h2>
               </div>
               
               <div className="buttons-container">
                    <Link to="/login" className="login">
                        <img src={loginIcon} alt="Login" />
                        Login
                    </Link> 

                    <Link to="/admin" className="admin">
                        <img src={adminIcon} alt="Admin Login" />
                        Admin
                    </Link>
               </div>
           </div>
       </div> 
    )
}

export default Landing;