import { request, Router, response } from 'express';
import dgram from 'dgram';
// import { DevicesRepository } from "./repositories/DevicesRepository";
// import { getCustomRepository } from "typeorm";


import { AdminsController } from './controllers/AdminsController';
import { DevicesController } from './controllers/DevicesController';
import { KeysController } from './controllers/KeysController';
import { SocketIDsController } from './controllers/SocketIDController';
import { ConnectionsController } from './controllers/ConnectionsController';
import { PatientsController } from './controllers/PatientsController';

const routes = Router();

const devicesController = new DevicesController();
const keysController = new KeysController();
const socket_idController = new SocketIDsController();
const adminsController = new AdminsController();
const connectionsController = new ConnectionsController();
const patientsController = new PatientsController();

routes.post('/admin', adminsController.create);

routes.post('/devices', devicesController.create);
routes.get('/devices/:id', devicesController.showByDeviceID);
// routes.get('/devices', devicesController.list);

routes.post('/keys', keysController.create);

routes.post('/socketid', socket_idController.create);

routes.post('/connections', connectionsController.create);
routes.get('/connections', connectionsController.list);

routes.post('/patients', patientsController.create);
routes.get('/patients/:id', patientsController.showByPatientID);
// routes.get('/patients', patientsController.list);


// routes.get('/admin', adminsController.list);
// routes.get('/admin', (request, response) => {

// });

export { routes };

/**
 * Tipos de parametros:
 * Routes Params => Parametros de rotas
 *  http://localhost:3333/settings/1
 *
 * Query Param =>  Filtros e buscas
 *  http://localhost:3333/settings/1?search=algumacoisa
 *
 * Body params =>  {
 *  passa json
 * }
 *
 */
