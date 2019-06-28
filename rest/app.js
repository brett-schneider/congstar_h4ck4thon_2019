const express = require('express')
const app = express()
const port = 3000
const fs = require('fs');
var b = -1;

app.get('/', (req, res) => {
	let bestand = fs.readFileSync('./bst').toString();
	console.log(bestand);
	return res.json({ bestand: bestand });
});
app.post('/', (req, res) => { 
	// console.log(req);
	const { bestand } = req.query
	fs.writeFile("./bst", bestand, function(err) {
    		if(err) { return console.log(err); }
    		console.log("The file was saved!"); 
	});
	res.sendStatus(200);
});
app.listen(port, () => console.log(`wDWH Ticker listening on port ${port}!`))
