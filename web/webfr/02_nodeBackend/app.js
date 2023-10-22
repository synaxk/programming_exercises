const db = require('./db.js')
const express = require('express');
const bodyParser = require('body-parser');


const app = express();

app.use((req, res, next) => {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Allow-Credentials", "true");
    res.setHeader("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS, POST, PUT, PATCH");
    res.setHeader("Access-Control-Allow-Headers",
        "Access-Control-Allow-Headers, Origin, Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers");
    next();
});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

app.post('/login', (req, res, next) => {
    const loginData = req.body;
    const cred = db.login(loginData["email"], loginData["password"]);
    if (cred !== null) {
        res.status(201).json(cred);
    } else {
        res.status(401).json({
            message: "Invalid username or password"
        });
    }
});

app.post('/signup', (req, res, next) => {
    const signupData = req.body;
    console.log(signupData);
    const signup = db.signup(signupData["email"], signupData["password"]);
    if (signup) {
        res.status(201).json({
            message: "Signup successful."
        });
    } else {
        res.status(400).json({
            message: "Email already exists."
        });
    }
})

module.exports = app;