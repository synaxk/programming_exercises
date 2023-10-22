const mongoose = require('mongoose');

const userDataSchema = mongoose.Schema({
    username: String,
    password: String
});

module.exports = mongoose.model('UserData', userDataSchema);
