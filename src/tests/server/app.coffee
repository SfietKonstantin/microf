express = require 'express'
morgan = require 'morgan'
bodyParser = require 'body-parser'
http = require 'http'

app = express()
app.set 'port', 8080
app.use morgan('dev')
app.use bodyParser.urlencoded({extended: 'true'})
app.use bodyParser.json()
app.use bodyParser.json({type: 'application/vnd.api+json'})

# routes
app.post '/api/postsimple', (req, res) ->
    res.json {'text': 'Hello world'}

app.post '/api/postsimpleerror', (req, res) ->
    res.status(401).json {'error': 'Some failure'}

server = http.createServer(app).listen app.get('port')
console.log("Server started on port #{app.get('port')}")
