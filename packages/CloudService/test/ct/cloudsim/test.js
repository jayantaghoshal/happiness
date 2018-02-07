var jsonServer = require('/usr/local/lib/node_modules/json-server')
var js2xmlparser = require('/usr/local/lib/node_modules/js2xmlparser')
var server = jsonServer.create()
var router = jsonServer.router('./db.json')
var middlewares = jsonServer.defaults()

const low = require('/usr/local/lib/node_modules/lowdb')
const FileSync = require('/usr/local/lib/node_modules/lowdb/adapters/FileSync')

var db = router.db

var routes = require('./routes.json')
server.use(jsonServer.rewriter(routes))

server.use(middlewares)
server.use(jsonServer.bodyParser)

router.render = function (req, res) {
  var data = res.locals.data
  var tag = ""
  if (req._parsedUrl.pathname == '/entrypoint') {
    tag = "entry_point"
  } else if (req._parsedUrl.pathname == '/features-1') {
    tag = "features"
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1') {
    tag = "software_management"
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1/') {
    tag = "software_management"
  } else if (req._parsedUrl.pathname == '/availableUpdates') {
    tag = "available_updates"
  } else if (req._parsedUrl.pathname == '/commission') {
    tag = "commission"
  } else {
    return res.send(data)
  }
  var str = js2xmlparser.parse(tag, data)
  res.send(str)
}

server.post('/availableUpdates', function (req, res, next) {
  return res.status(405).send({ error: "Nope, cant do that." })
})

server.post('/commission', function (req, res, next) {
  var t = db.get('availableUpdates').get('software')

  for (i = 0; i < t.value().length; i++) {
    if (t.value()[i]['id'] == req.body.id) {
      var tmp = t.value()[i]
      db.get('downloads').get('software').push(tmp).write()
      db.get('availableUpdates').get('software').remove(tmp).write()
      break
    }
  }

  req.method = 'GET'
  next()
})

server.use(router)
server.listen(3000, function () {
  console.log('JSON Server is running')
})