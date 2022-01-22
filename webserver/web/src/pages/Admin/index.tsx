import React, {useState, FormEvent} from "react";

import api from "../../services/api";

import './styles.css';

function Admin() {
    // const [username, setUsername] = useState('');
    const handleSubmit = async () => {

        try {
            const response =  await api.get('/admin',{})

            console.log(response);
        } catch (error) {
            console.error(error);
        }
    }
    // onClick={handleSubmit}


    return (
        <div id="page-admin">
            <h1>Rota do admin</h1>
            <button id="button-connection" onClick={handleSubmit} type="button">iniciar conexão</button> 
        </div>
    )
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