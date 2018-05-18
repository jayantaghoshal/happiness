var jsonServer = require('/usr/lib/node_modules/json-server');
var js2xmlparser = require('/usr/lib/node_modules/js2xmlparser');
var server = jsonServer.create();
var router = jsonServer.router(__dirname + '/db.json');
var middlewares = jsonServer.defaults({ static: __dirname + '/public' });

var db = router.db;

var routes = require(__dirname + '/routes.json');
server.use(jsonServer.rewriter(routes));

server.use(middlewares);
server.use(jsonServer.bodyParser);

router.render = function (req, res) {
  var data = res.locals.data;
  var tag = "";
  if (req._parsedUrl.pathname == '/entrypoint') {
    tag = "entry_point";
  } else if (req._parsedUrl.pathname == '/features-1') {
    tag = "features";
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1') {
    tag = "software_management";
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1/') {
    tag = "software_management";
  } else if (req._parsedUrl.pathname == '/availableUpdates') {
    tag = "available_updates";
  } else if (req._parsedUrl.pathname == '/availableAccessories') {
    tag = "available_accessories";
  } else if (req._parsedUrl.pathname == '/commission') {
    tag = "commission";
  } else if (req._parsedUrl.pathname == '/downloads') {
    tag = "downloads";
  }
  else {
    return res.send(data);
  }
  var str = js2xmlparser.parse(tag, data);
  res.send(str);
}

server.post('/availableUpdates', function (req, res, next) {
  return res.status(405).send({ error: "Nope, cant do that." });
});


var counter = -1;
server.post('/commission', function (req, res, next) {

  var updates = db.get('availableUpdates').get('software');
  var accessories = db.get('availableAccessories').get('software');
  var found = false;

  for (i = 0; i < updates.value().length; i++) {
    if (updates.value()[i]['id'] == req.body.id) {
      found = true;
      var tmp = updates.value()[i];

      counter++;
      addInstallationOrder(tmp, createInstallationOrderId(counter));

      for (i = 0; i < updates.value().length; i++) {
        if (updates.value()[i]['id'] != req.body.id) {
          var tmp2 = updates.value()[i];
          tmp2['status'] = "NON-COMMISSIONABLE";
        }
      }

      for (i = 0; i < accessories.value().length; i++) {
        var tmp2 = accessories.value()[i];
        tmp2['status'] = "NON-COMMISSIONABLE";
      }

      db.get('availableUpdates').get('software').write();
      db.get('availableAccessories').get('software').write();

      break;
    }
  }

  if (!found) {
    for (i = 0; i < accessories.value().length; i++) {
      if (accessories.value()[i]['id'] == req.body.id) {
        found = true;
        var tmp = accessories.value()[i];

        counter++;
        addInstallationOrder(tmp, createInstallationOrderId(counter));

        for (i = 0; i < accessories.value().length; i++) {
          if (accessories.value()[i]['id'] != req.body.id) {
            var tmp2 = accessories.value()[i];
            tmp2['status'] = "NON-COMMISSIONABLE";
          }
        }

        for (i = 0; i < updates.value().length; i++) {
          var tmp2 = updates.value()[i];
          tmp2['status'] = "NON-COMMISSIONABLE";
        }

        db.get('availableUpdates').get('software').write();
        db.get('availableAccessories').get('software').write();

        break;
      }
    }
  }

  if (found) {
    var d = db.get('preDownloadInfo');
    for (j = 0; j < d.value().length; j++) {
      if (d.value()[j]['id'] == req.body.id) {
        var tmpObj = Object.assign({}, d.value()[j]);
        tmpObj['id'] = req.body.id;
        tmpObj['installation_order_id'] = createInstallationOrderId(counter);
        db.get('downloads').push(tmpObj).write();
        break;
      }
    }
  }

  req.method = 'GET';
  next();
});

function createInstallationOrderId(counter) {
  var counter_uuid_format_prefix = counter.toString(16);
      while (counter_uuid_format_prefix.length < 8) {
        counter_uuid_format_prefix = '0' + counter_uuid_format_prefix;
      }
      var id_suffix ="-caf6-4710-05a9-472200ad680c";
      var counter_uuid_format = counter_uuid_format_prefix + id_suffix;
      return counter_uuid_format;
}

function addInstallationOrder(obj, id) {
  var installation_order_data = {};
  installation_order_data['id'] = id;
  installation_order_data['status'] = "READY";
  installation_order_data['created_by'] = "1FTKR1EDXBPB10452";
  installation_order_data['created'] = "2002-05-30T09:00:00";
  installation_order_data['downloads_uri'] = "/downloads/";
  installation_order_data['install_notifications_uri'] = "installnotifications";
  installation_order_data['installation_report_uri'] = "/installationreport";

  obj['installation_order'] = installation_order_data
  obj['status'] = "COMMISSIONED";
  return obj;
}

server.get('/downloads', function (req, res, next) {
  var d = db.get('downloads');
  for (j = 0; j < d.value().length; j++) {
    if (d.value()[j]['installation_order_id'] == req.query.id) {
      var tmpObj = d.value()[j];
      var str = js2xmlparser.parse("downloads", tmpObj);
      return res.send(str);
    }
  }

  return res.status(405).send({ error: "Nope, cant do that." });
});

function getValue(xml, tag) {
  var startTag = '<' + tag + '>';
  var endTag = '</' + tag + '>';
  startPos = xml.indexOf(startTag) + startTag.length;
  endPos = xml.indexOf(endTag);
  return xml.substring(startPos, endPos);
}

var idCounter = 1000;
server.post('/installNotification', function (req, res, next) {
  var t = {};
  var obj = {};

  t = req.body;
  var xml = t['<?xml version'];

  var tag = "id";
  obj['software_id'] = getValue(xml, tag);

  tag = "installation_order_id";
  obj[tag] = getValue(xml, tag);

  var notificationObj = {};
  tag = "status_code";
  notificationObj[tag] = getValue(xml, tag);

  tag = "sub_status_code";
  notificationObj[tag] = getValue(xml, tag);

  notificationObj['id'] = idCounter;
  idCounter++;
  notificationObj['created'] = "2017-05-30T09:00";
  notificationObj['timestamp'] = "2017-05-30T09:00";
  obj['notification'] = notificationObj;

  db.get('installNotification').push(obj).write();

  req.method = 'GET';
  next();
})


//BROKEN! NEEDS TO BE FIXED
server.get('/installNotification', function (req, res, next) {
  var d = db.get('installNotification');
  for (j = 0; j < d.value().length; j++) {
    if (d.value()[j]['installation_order_id'] == req.query.installation_order_id) {
      var tmpObj = d.value()[j];
      var str = js2xmlparser.parse("install_notifications", tmpObj);
    }
  }

  d = db.get('downloads');
  for (j = 0; j < d.value().length; j++) {
    if (d.value()[j]['installation_order_id'] == req.query.installation_order_id) {
      var tmpObj = d.value()[j];
      d.remove(tmpObj).write();
    }
  }

  return res.send(str);
});

function count(main_str, sub_str) {
  main_str += '';
  sub_str += '';

  if (sub_str.length <= 0) {
    return main_str.length + 1;
  }

  subStr = sub_str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  return (main_str.match(new RegExp(subStr, 'gi')) || []).length;
}

server.post('/installationReport', function (req, res, next) {
  var t = {};
  var obj = {};
  t = req.body;
  var xml = t['<?xml version'];

  var tag = "installation_order_id";
  obj[tag] = getValue(xml, tag);
  var installation_order_id = getValue(xml, tag);
  console.log(installation_order_id)

  tag = "timestamp";
  obj[tag] = getValue(xml, tag);

  tag = "report_reason";
  obj[tag] = getValue(xml, tag);

  var downloadSummaryObj = {};
  var downloadSummary = getValue(xml, "download_summary");
  tag = "timestamp";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  tag = "total_download_time";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  tag = "effective_download_time";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  var i = 0;
  var subXml = getValue(downloadSummary, "data_file");
  var dataFilesArray = []
  while (i != count(downloadSummary, "<data_file>")) {
    var dataFileObj = {};
    var dataFile = subXml;
    tag = "identifier";
    dataFileObj[tag] = getValue(dataFile, tag);

    tag = "target_storage_id";
    dataFileObj[tag] = getValue(dataFile, tag);

    tag = "status";
    dataFileObj[tag] = getValue(dataFile, tag);
    dataFilesArray[i] = dataFileObj;

    ++i;
    subXml = downloadSummary.substring(downloadSummary.indexOf(subXml) + subXml.length + "</data_file>".length);
    subXml = getValue(subXml, "data_file");
  }

  downloadSummaryObj['data_file'] = dataFilesArray;
  obj["download_summary"] = downloadSummaryObj;

  var installationSummaryObj = {};
  var installationSummary = getValue(xml, "installation_summary");

  tag = "software_id";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "timestamp";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "repeat_resets";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "total_installation_time";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  i = 0;
  subXml = getValue(installationSummary, "ecu");
  var ecuArray = [];

  while (i != count(installationSummary, "<ecu>")) {
    var ecuObj = {}
    var ecu = subXml;

    tag = "ecu_address";
    ecuObj[tag] = getValue(ecu, tag);

    tag = "ecu_retries";
    ecuObj[tag] = getValue(ecu, tag);

    tag = "ecu_status";
    ecuObj[tag] = getValue(ecu, tag);

    j = 0;
    var subSubXml = getValue(ecu, "software_part");
    var softwarepartArray = [];
    while (j != count(ecu, "<software_part>")) {
      var softwarepartObj = {};
      var softwarepart = subSubXml;

      tag = "identifier";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "retries";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "measured_installation_time";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "status";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      softwarepartArray[j] = softwarepartObj;
      ++j;
      subSubXml = ecu.substring(ecu.indexOf(subSubXml) + subSubXml.length + "</software_part>".length);
      subSubXml = getValue(subSubXml, "software_part");
    }
    ecuObj["software_part"] = softwarepartArray;
    ecuArray[i] = ecuObj;
    ++i;
    subXml = ecu.substring(ecu.indexOf(subXml) + subXml.length + "</ecu>".length);
    subXml = getValue(subXml, "ecu");
  }
  installationSummaryObj['ecu'] = ecuObj;
  obj["installation_summary"] = installationSummaryObj;
  db.get("installationReport").push(obj).write();

  req.method = 'GET';
  next();

  //"Close" installation order by removing from available updates and make other assignments COMMISSIONABLE
  var t = db.get('availableUpdates').get('software');
  for (i = 0; i < t.value().length; i++) {
    if (t.value()[i]['status'] == 'COMMISSIONED') {
      if (t.value()[i]['installation_order']['id'] == installation_order_id) {
        var tmp = t.value()[i];
        t.remove(tmp).write();
      }
    }
  }
  db.get('availableUpdates').get('software').write();
  t = db.get('availableUpdates').get('software');

  for (i = 0; i < t.value().length; i++) {
    var tmp2 = t.value()[i];
    tmp2['status'] = "COMMISSIONABLE";
  }

  db.get('availableUpdates').get('software').write();

  //"Close" installation order by removing from available accessories and make other assignments COMMISSIONABLE
  var t = db.get('availableAccessories').get('software');
  for (i = 0; i < t.value().length; i++) {
    if (t.value()[i]['status'] == 'COMMISSIONED') {
      if (t.value()[i]['installation_order']['id'] == installation_order_id) {
        var tmp = t.value()[i];
        t.remove(tmp).write();
      }
    }
  }
  db.get('availableAccessories').get('software').write();
  t = db.get('availableAccessories').get('software');

  for (i = 0; i < t.value().length; i++) {
    var tmp2 = t.value()[i];
    tmp2['status'] = "COMMISSIONABLE";
  }

  db.get('availableAccessories').get('software').write();
});

server.get('/availableUpdates', function (req, res, next) {
  var d = db.get('availableUpdates').get('software');
  var tmpObj = {};
  if (req.query.id) {
    for (i = 0; i < d.value().length; i++) {
      if (req.query.installation_order_id && d.value()[i]['installation_order']) {
        if ((d.value()[i]['id'] == req.query.id) && (d.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
          tmpObj['software'] = d.value()[i];
        }
      }
      else if (!req.query.installation_order_id) {
        if (d.value()[i]['id'] == req.query.id) {
          tmpObj['software'] = d.value()[i];
        }
      }
    }
  }

  else if (req.query.installation_order_id) {
    for (i = 0; i < d.value().length; i++) {
      if ((d.value()[i]['installation_order']) && (d.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
        tmpObj['software'] = d.value()[i];
      }
    }
  }
  else {
    tmpObj['software'] = d.value()
  }

  var str = js2xmlparser.parse("available_updates", tmpObj);
  return res.send(str);
});

server.get('/availableAccessories', function (req, res, next) {
  var d = db.get('availableAccessories').get('software');
  var tmpObj = {};
  if (req.query.id) {
    for (i = 0; i < d.value().length; i++) {
      if (req.query.installation_order_id && d.value()[i]['installation_order']) {
        if ((d.value()[i]['id'] == req.query.id) && (d.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
          tmpObj['software'] = d.value()[i];
        }
      }
      else if (!req.query.installation_order_id) {
        if (d.value()[i]['id'] == req.query.id) {
          tmpObj['software'] = d.value()[i];
        }
      }
    }
  }

  else if (req.query.installation_order_id) {
    for (i = 0; i < d.value().length; i++) {
      if ((d.value()[i]['installation_order']) && (d.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
        tmpObj['software'] = d.value()[i];
      }
    }
  }
  else {
    tmpObj['software'] = d.value()
  }

  var str = js2xmlparser.parse("available_accessories", tmpObj);
  return res.send(str);
});

server.use(router);
server.listen(3000, function () {
  console.log('JSON Server is running');
});