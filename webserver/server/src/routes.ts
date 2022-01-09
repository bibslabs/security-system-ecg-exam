import { Router } from "express";
import { getCustomRepository } from "typeorm";

import { DevicesRepository } from "./repositories/Devicesrepository";

const routes = Router();

routes.post("/devices", async(request, response) => {
    const {size, token, socket_address} = request.body;
    
    const devicesRepository = getCustomRepository(DevicesRepository);

    const devices = devicesRepository.create({
        size,
        token,
        socket_address
    })

    await devicesRepository.save(devices)
});

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