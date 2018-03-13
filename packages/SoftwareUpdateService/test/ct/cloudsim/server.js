var jsonServer = require('/usr/local/lib/node_modules/json-server')
var js2xmlparser = require('/usr/local/lib/node_modules/js2xmlparser')
var server = jsonServer.create()
var router = jsonServer.router(__dirname + '/db.json')
var middlewares = jsonServer.defaults({static:__dirname + '/public'})

var db = router.db

var routes = require(__dirname + '/routes.json')
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
  } else if(req._parsedUrl.pathname == '/pendingInstallations') {
    tag = "pending_installations"
  } else if(req._parsedUrl.pathname == '/downloads') {
    tag = "downloads"
  }
   else {
    return res.send(data)
  }
  var str = js2xmlparser.parse(tag, data)
  res.send(str)
}

server.post('/availableUpdates', function (req, res, next) {
  return res.status(405).send({ error: "Nope, cant do that." })
})


var counter = -1
server.post('/commission', function (req, res, next) {

  var t = db.get('availableUpdates').get('software')

  for (i = 0; i < t.value().length; i++) {
    if (t.value()[i]['id'] == req.body.id) {
      var tmp = t.value()[i]

      counter++

      var installation_order_data = {}
      installation_order_data['id'] = counter
      installation_order_data['status'] = "PENDING"
      installation_order_data['created_by'] = "1FTKR1EDXBPB10452"
      installation_order_data['created'] = "2002-05-30T09:00:00"
      installation_order_data['downloads_uri'] = "/downloads/" //+ counter
      installation_order_data['install_notifications_uri'] = "/installNotification"
      installation_order_data['installation_report_uri'] = "/installationreport"
      installation_order_data['software'] = [tmp]
      var installation_order = {}
      installation_order['installation_order'] = installation_order_data
      db.get('pendingInstallations').get('installation_order').push(installation_order_data).write();
      db.get('availableUpdates').get('software').remove(tmp).write()


      var d = db.get('preDownloadInfo')
      for (j = 0; j < d.value().length; j++) {
        if (d.value()[j]['id'] == req.body.id) {
          var tmpObj = Object.assign({}, d.value()[j])
          tmpObj['id'] = req.body.id
          tmpObj['installation_order_id'] = counter
          db.get('downloads').push(tmpObj).write()
          break
        }
      }
      break
    }
  }

  req.method = 'GET'
  next()
})

server.get('/downloads', function(req, res, next) {
  var d = db.get('downloads')
      for (j = 0; j < d.value().length; j++) {
        if (d.value()[j]['installation_order_id'] == req.query.id) {
          var tmpObj = d.value()[j]
          var str = js2xmlparser.parse("downloads", tmpObj)
          return res.send(str)
        }
      }

      return res.status(405).send({ error: "Nope, cant do that." })
})

server.post('/installNotification', function (req, res, next) {
  req.method = 'GET'
  next()
})

server.use(router)
server.listen(3000, function () {
  console.log('JSON Server is running')
})