const mongoose = require('mongoose');

const envs = require('../shared/env');

mongoose.connect(String(envs.DATABASE_URL), {
  useUnifiedTopology: true,
  useNewUrlParser: true
});