const dotenv = require("dotenv");

dotenv.config();

const env =  {
    DATABASE_URL: process.env.DATABASE_URL, 
    PORT: process.env.PORT
} 

module.exports = env;