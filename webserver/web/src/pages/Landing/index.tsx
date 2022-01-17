import React from "react";

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
                    <a href="" className="login">
                        <img src={loginIcon} alt="Login" />
                        Login
                    </a> 

                    <a href="" className="admin">
                        <img src={adminIcon} alt="Admin Login" />
                        Admin
                    </a>
               </div>
           </div>
       </div> 
    )
}

export default Landing;