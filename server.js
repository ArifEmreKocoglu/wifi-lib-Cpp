const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');

const app = express();

app.use(bodyParser.json());
app.use(express.static('public'));  // public klasörü için statik dosya sunucusu

app.get('/scan', (req, res) => {
    exec('./main scan', (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

app.get('/currentNet', (req, res) => {
    exec('./main currentnet', (error, stdout, stderr) => {
        if (error){
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

app.get('/netDetails', (req, res) => {
    exec('./main netdetails', (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

app.get('/speedTest', (req, res) => {
    exec('./main speedtest', (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

app.post('/connect', (req, res) => {
    const ssid = req.body.ssid;
    console.log(ssid);
    const password = req.body.password;
    console.log(password);
    exec(`./main connect ${ssid} ${password}`, (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

app.post('/disconnect', (req, res) => {
    exec('./main disconnect', (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(stderr);
        } else {
            res.send(stdout);
        }
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
