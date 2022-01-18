import * as React from 'react';

import { BrowserRouter, Route, Routes } from "react-router-dom";
import Admin from './pages/Admin';
import Landing from './pages/Landing';
import Login from './pages/Login';

function Rotas() {
	return (
		<BrowserRouter>
			<Routes>
				<Route path="/" element={<Landing />}/>
				<Route path="/login" element={<Login />}/>
				<Route path="/admin" element={<Admin />}/>
				{/* <Route path="/login">
					<Login />
				</Route>
				<Route path="/admin">
					<Admin />
				</Route> */}
			</Routes>
		</BrowserRouter>
	);
}

export default Rotas;
