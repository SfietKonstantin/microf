express = require 'express'
morgan = require 'morgan'
bodyParser = require 'body-parser'
http = require 'http'

app = express()
app.set 'port', 8080
app.use morgan('dev')
app.use bodyParser.urlencoded({extended: 'true'})
app.use bodyParser.json()

# routes
app.post '/api/postsimple', (req, res) ->
    res.json {'text': 'Hello world'}

app.post '/api/postsimpleerror', (req, res) ->
    res.status(401).json {'error': 'Some failure'}

app.post '/api/postsimplelist', (req, res) ->
    if 'next' of req.body
        res.json {'values': [{id: 2, text: "Entry 2"}, {id: 3, text: "Entry 3"}]}
    else
        res.json {'values': [{id: 0, text: "Entry 0"}, {id: 1, text: "Entry 1"}], 'have_next': true}

server = http.createServer(app).listen app.get('port')
console.log("Server started on port #{app.get('port')}")
