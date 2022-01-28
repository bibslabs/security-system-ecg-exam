import { request, Router, response } from 'express';
// import { DevicesRepository } from "./repositories/DevicesRepository";
// import { getCustomRepository } from "typeorm";
import { AdminsController } from './controllers/AdminsController';
import { DevicesController } from './controllers/DevicesController';
import { KeysController } from './controllers/KeysController';
import { ConnectionsController } from './controllers/ConnectionsController';
import { PatientsController } from './controllers/PatientsController';

const routes = Router();

const devicesController = new DevicesController();
const keysController = new KeysController();
const adminsController = new AdminsController();
const connectionsController = new ConnectionsController();
const patientsController = new PatientsController();

routes.post('/admin', adminsController.create);
routes.post('/devices', devicesController.create);
routes.post('/keys', keysController.create);
routes.post('/connections', connectionsController.create);
routes.post('/patients', patientsController.create);

routes.get('/devices', devicesController.list);
routes.get('/connections', connectionsController.list);
routes.get('/patients', patientsController.list);
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
