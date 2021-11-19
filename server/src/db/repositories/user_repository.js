const User = require("../models/user");

class UserRepository{
    create({userName, password}){
        if (!userName || !password) {
            throw new Error("Nome de usuário ou Senha não podem ser vazios.");
        }
        return User.create({userName, password});
    }
}

module.exports = UserRepository;