import { request, Router } from 'express';
// import { DevicesRepository } from "./repositories/DevicesRepository";
// import { getCustomRepository } from "typeorm";
import { DevicesController } from './controllers/DevicesController';
import { KeysController } from './controllers/KeysController';

const routes = Router();

const devicesController = new DevicesController();
const keysController = new KeysController();

routes.post('/devices', devicesController.create);
routes.post('/keys', keysController.create);

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
