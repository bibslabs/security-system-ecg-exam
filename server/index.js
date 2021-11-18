const app = require('./app');
const appWs = require('./app-ws');
const env = require('./src/shared/env');
require('./src/db/connection');
 
const server = app.listen(env.PORT || 3000, () => {
    console.log(`App Express is running!`);
})
 
appWs(server);

