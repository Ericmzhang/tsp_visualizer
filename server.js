
const express = require('express');
const fs = require('fs');
const path = require('path');
const cors = require('cors');
const { spawn } = require('child_process');
const multer = require('multer')
const { request } = require('http');
const app = express();
const port = 3001;
const upload = multer({ dest: 'uploads/' });
// Middleware for parsing JSON and urlencoded form data
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(cors())

let coordinates = [

]

app.post('/run-cpp-txt', upload.single('file'), (req, res) => {
    if (!req.file) {
        return res.status(400).send('No file uploaded.');
    }

    const filePath = req.file.path;
    const child = spawn('./src/tsp', ['-m', 'OPTTSP'], { stdio: ['pipe', 'pipe', 'pipe'] });

    const fileStream = fs.createReadStream(filePath);
    fileStream.pipe(child.stdin);

    let output = '';
    child.stdout.on('data', (data) => {
        output += data.toString();
    });

    child.on('close', (code) => {
        console.log(`Child process exited with code ${code}`);
        fs.unlinkSync(filePath); // Clean up uploaded file
        res.send(output);
    });

    child.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    child.on('error', (error) => {
        console.error(`Failed to start subprocess: ${error}`);
        res.status(500).send('Error executing C++ program');
    });
});

app.post('/run-cpp-manual', (req, res) => {
    const data = req.body.data; // Get data from request body
    const child = spawn('./src/tsp', ['-m', 'OPTTSP'], { stdio: ['pipe', 'pipe', 'pipe'] });

    child.stdin.write(data); // Write data to child's stdin
    child.stdin.end();

    let output = '';
    child.stdout.on('data', (data) => {
        output += data.toString();
    });
    

    child.on('close', (code) => {
        console.log(`Child process exited with code ${code}`);
        res.send(output);
    });

    child.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    child.on('error', (error) => {
        console.error(`Failed to start subprocess: ${error}`);
        res.status(500).send('Error executing C++ program');
    });
});

app.get('/result', (req, res) => {
    
    // Using path.join to create the relative path
    const filePath = path.join(__dirname, 'src', 'output.txt');
    console.log(filePath)
    fs.readFile(filePath, 'utf8', (err, data) => {
        if (err) {
            res.status(500).send("Error reading the file");
            return;
        }
        res.send(data);
    });
});

app.get('/data', (req, res) => {
    res.json(coordinates)
});

app.post('/data', (req,res)=> {
    
    coordinates = req.body
    res.json(req.body)

    // Process your data here...
})

app.post('/clear-data', (req,res)=> {
    console.log("clearing data")
    coordinates = []
    res.status(200).send('Data cleared');

    // Process your data here...
})

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});