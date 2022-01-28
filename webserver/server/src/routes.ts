import { request, Router, response } from 'express';
// import { DevicesRepository } from "./repositories/DevicesRepository";
// import { getCustomRepository } from "typeorm";
import { AdminsController } from './controllers/AdminsController';
import { DevicesController } from './controllers/DevicesController';
import { KeysController } from './controllers/KeysController';
import { ConnectionsController } from './controllers/ConnectionsController';

const routes = Router();

const devicesController = new DevicesController();
const keysController = new KeysController();
const adminsController = new AdminsController();
const connectionsController = new ConnectionsController();

routes.post('/admin', adminsController.create);
routes.post('/devices', devicesController.create);
routes.post('/keys', keysController.create);
routes.post('/connections', connectionsController.create);

routes.get('/devices', devicesController.list);
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
